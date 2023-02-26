/*
BSD 3-Clause License

Copyright (c) 2022, Thomas DiModica
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "Backwards/Input/Lexer.h"
#include "Backwards/Input/StringInput.h"

#include "Backwards/Parser/SymbolTable.h"
#include "Backwards/Parser/Parser.h"

#include "Backwards/Engine/ConstantsSingleton.h"
#include "Backwards/Engine/Expression.h"
#include "Backwards/Engine/DebuggerHook.h"
#include "Backwards/Engine/FunctionContext.h"
#include "Backwards/Engine/FatalException.h"
#include "Backwards/Engine/Logger.h"
#include "Backwards/Engine/ProgrammingException.h"
#include "Backwards/Engine/DebuggerHook.h"
#include "Backwards/Engine/StackFrame.h"

#include "Backwards/Types/FloatValue.h"
#include "Backwards/Types/StringValue.h"
#include "Backwards/Types/ArrayValue.h"
#include "Backwards/Types/DictionaryValue.h"
#include "Backwards/Types/FunctionValue.h"

#include "Backway/CallingContext.h"
#include "Backway/Environment.h"
#include "Backway/StateMachine.h"

#include "Commands.h"

#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>

#define OLC_KEYBOARD_US
#define OLC_PGE_APPLICATION
#include "include/olcPixelGameEngine.h"

#include "Zone.h"
void getStart(int, Zone&, Zone&);
std::string makeMap(unsigned int x, unsigned int y, unsigned int z);

const int WORLD_WIDTH = 512;
const int WORLD_HEIGHT = 512;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE = 16;

class ConsoleLogger final : public Backwards::Engine::Logger
 {
public:
   std::stringstream& sink;
   ConsoleLogger(std::stringstream& sink) : sink(sink) { }
   void log (const std::string& message) { sink << message << std::endl; }

   std::condition_variable reader;
   std::mutex lock;
   std::list<std::string> list;
   std::string get ()
    {
      std::unique_lock<std::mutex> scoped(lock);
      while (true == list.empty())
       {
         reader.wait(scoped);
       }
      std::string result = list.front();
      list.pop_front();
      scoped.unlock();
      return result;
    }
   void put(const std::string& str)
    {
       {
         std::unique_lock<std::mutex> scoped(lock);
         list.push_back(str);
       }
      reader.notify_one();
    }
 };

class NullLogger final : public Backwards::Engine::Logger
 {
public:
   void log (const std::string&) { return; }
   std::string get () { return ""; }
 };

class DebuggerLogger final : public Backwards::Engine::Logger
 {
public:
   std::stringstream& sink;
   std::vector<std::string> commands;
   size_t command;
   DebuggerLogger(std::stringstream& sink) : sink(sink) { }
   void log (const std::string& message) { sink << message << std::endl; }
   std::string get () { if (command >= commands.size()) return "quit"; return commands[command++]; }
 };
DebuggerLogger* nastyHack;

class ContextBuilder final
 {
public:
   static void createGlobalScope(Backwards::Engine::Scope&);
 };

class Ayers
 {
public:
   std::string m;
   uint64_t a;
 };

const std::size_t cacheMax = 100U;
std::map<Zone, Ayers> cache;
const std::string& getCache(unsigned int x, unsigned int y, unsigned int z)
 {
   static uint64_t h = 0U;
   ++h;
   Zone zone;
   zone.x = x;
   zone.y = y;
   zone.z = z;

   if (cache.end() == cache.find(zone))
    {
      cache[zone].m = makeMap(x, y, z);
    }
   cache[zone].a = h;

   if (cache.size() > cacheMax)
    {
      std::map<Zone, Ayers>::iterator found = cache.begin();
      uint64_t b = found->second.a;
      for (std::map<Zone, Ayers>::iterator iter = cache.begin(); cache.end() != iter; ++iter)
       {
         if (iter->second.a < b)
          {
            b = iter->second.a;
            found = iter;
          }
       }
      cache.erase(found);
    }

   return cache[zone].m;
 }

class View : public olc::PixelGameEngine
 {
public:
   View() : logger (ConsoleOut()), debugLogger(ConsoleOut())
    {
      sAppName = "Backroom Quest Alpha v0.1.1";
    }

private:
   std::vector<std::unique_ptr<olc::Sprite> > floors;
   std::vector<std::unique_ptr<olc::Sprite> > doors;
   std::unique_ptr<olc::Sprite> players;
   Zone zone, player;
   double counter;
   bool mu, md, ml, mr;
   unsigned int px, py;
   int sc_x, sc_y;

   Backwards::Engine::Scope global;
   ConsoleLogger logger;
   NullLogger nullLogger;
   DebuggerLogger debugLogger;
   Backwards::Engine::DefaultDebugger debugger;

   Backway::CallingContext context;
   Backway::CallingContext nullLog;
   Backway::CallingContext nullDebug;
   Backway::StateMachine machine;
   Backway::Environment environment;

   void reset()
    {
      machine.states.clear();
      machine.last = Backwards::Engine::ConstantsSingleton::getInstance().EMPTY_DICTIONARY;
      machine.input = Backwards::Engine::ConstantsSingleton::getInstance().FLOAT_ZERO;
      machine.output = std::shared_ptr<Backway::Command>();
    }

public:
   bool OnUserCreate() override
    {
      // Called once at the start, so create things here
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/E.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/T.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/R.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/TR.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/B.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/TB.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/BR.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/TBR.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/L.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/TL.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/LR.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/TLR.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/BL.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/TBL.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/BLR.png"));
      floors.emplace_back(std::make_unique<olc::Sprite>("Tiles/TBLR.png"));
      doors.emplace_back(std::make_unique<olc::Sprite>("Tiles/VD.png"));
      doors.emplace_back(std::make_unique<olc::Sprite>("Tiles/HD.png"));
      players = std::make_unique<olc::Sprite>("Tiles/P.png");

      getStart(std::time(nullptr), zone, player);
      getCache(zone.x, zone.y, zone.z);

      counter = 0.0;
      mu = false;
      md = false;
      ml = false;
      mr = false;

      px = 65536;
      py = 65536;

      sc_x = 0;
      sc_y = 0;


      ContextBuilder::createGlobalScope(global);

      context.logger = &logger;
      context.debugger = &debugger;
      context.globalScope = &global;
      context.machine = &machine;
      context.environment = &environment;

      nullLog.logger = &nullLogger;
      nullLog.globalScope = &global;
      nullLog.machine = &machine;
      nullLog.environment = &environment;

      nullDebug.logger = &debugLogger;
      nastyHack = &debugLogger;
      nullDebug.debugger = &debugger;
      nullDebug.globalScope = &global;
      nullDebug.machine = &machine;
      nullDebug.environment = &environment;

      machine.rng = JAVA(std::time(nullptr));

      evaluateString("CreateState('__LEFT__'; 'set Called to 0 set Update to function left (arg) is if Called then call Leave() else call Left() set Called to 1 end return arg end')", nullLog);
      evaluateString("CreateState('__RIGHT__'; 'set Called to 0 set Update to function right (arg) is if Called then call Leave() else call Right() set Called to 1 end return arg end')", nullLog);
      evaluateString("CreateState('__UP__'; 'set Called to 0 set Update to function up (arg) is if Called then call Leave() else call Up() set Called to 1 end return arg end')", nullLog);
      evaluateString("CreateState('__DOWN__'; 'set Called to 0 set Update to function down (arg) is if Called then call Leave() else call Down() set Called to 1 end return arg end')", nullLog);

      reset();

      std::thread bob ( [this] { this->CommandThread(); });
      bob.detach();

      return true;
    }

   char getMapTile(int tx, int ty, const unsigned int z)
    {
      unsigned int x = zone.x;
      unsigned int y = zone.y;

      if (tx < 0)
       {
         x -= 1;
         tx += WORLD_WIDTH;
       }
      else if (tx >= WORLD_WIDTH)
       {
         x += 1;
         tx -= WORLD_WIDTH;
       }

      if (ty < 0)
       {
         y -= 1;
         ty += WORLD_HEIGHT;
       }
      else if (ty >= WORLD_HEIGHT)
       {
         y += 1;
         ty -= WORLD_HEIGHT;
       }

      return getCache(x, y, z)[ty * WORLD_WIDTH + tx];
    }

   int getTileNumber(char up, char right, char down, char left)
    {
      int result = 0;
      result |= (up != ' ') ? 1 : 0;
      result |= (right != ' ') ? 2 : 0;
      result |= (down != ' ') ? 4 : 0;
      result |= (left != ' ') ? 8 : 0;
      return result;
    }

   bool OnUserUpdate(float fElapsedTime) override
    {
      if (GetKey(olc::Key::ESCAPE).bPressed) { ConsoleShow(olc::Key::ESCAPE); return true; }
      if (0.0 == fElapsedTime) return true; // If we are showing the console, don't capture button presses.
      const int X_HALF = (SCREEN_WIDTH / TILE + 2) / 2;
      const int Y_HALF = (SCREEN_HEIGHT / TILE + 2) / 2;
      char map [Y_HALF * 2][X_HALF * 2];

      for (int y = -Y_HALF; y < Y_HALF; ++y)
       {
         for (int x = -X_HALF; x < X_HALF; ++x)
          {
            map[y + Y_HALF][x + X_HALF] = getMapTile(player.x + x, player.y + y, zone.z);
          }
       }

      if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::UP).bPressed) mu = true;
      else if (GetKey(olc::Key::S).bPressed || GetKey(olc::Key::DOWN).bPressed) md = true;
      else if (GetKey(olc::Key::D).bPressed || GetKey(olc::Key::RIGHT).bPressed) mr = true;
      else if (GetKey(olc::Key::A).bPressed || GetKey(olc::Key::LEFT).bPressed) ml = true;
      counter += fElapsedTime;
      int nsc_x = 0, nsc_y = 0;
      if (counter > 0.2)
       {
         bool moved = false;
         counter = 0.0;
         unsigned int ppx = player.x;
         unsigned int ppy = player.y;
         char nl = '#';

         if (GetKey(olc::Key::F1).bHeld) reset();
         else if (true == mu) evaluateString("Push('__UP__')", nullLog);
         else if (true == md) evaluateString("Push('__DOWN__')", nullLog);
         else if (true == mr) evaluateString("Push('__RIGHT__')", nullLog);
         else if (true == ml) evaluateString("Push('__LEFT__')", nullLog);

         try
          {
            machine.update(nullDebug);
          }
         catch (const Backwards::Types::TypedOperationException& e)
          {
            ConsoleOut() << "Caught runtime exception: " << e.what() << std::endl;
          }
         catch (const Backwards::Engine::FatalException& e)
          {
            ConsoleOut() << "Caught Fatal Error: " << e.what() << std::endl;
          }
         catch (const Backwards::Engine::ProgrammingException& e)
          {
            ConsoleOut() << "This is a BUG, please report it: " << e.what() << std::endl;
          }

         if (nullptr != machine.output.get())
          {
            if ((typeid(*machine.output) == typeid(Command_Up)) && ('#' != map[Y_HALF - 1][X_HALF])) { --player.y; nsc_y = -TILE; moved = true; nl = map[Y_HALF - 1][X_HALF]; }
            else if ((typeid(*machine.output) == typeid(Command_Down)) && ('#' != map[Y_HALF + 1][X_HALF])) { ++player.y; nsc_y = TILE; moved = true; nl = map[Y_HALF + 1][X_HALF]; }
            else if ((typeid(*machine.output) == typeid(Command_Right)) && ('#' != map[Y_HALF][X_HALF + 1])) { ++player.x; nsc_x = TILE; moved = true; nl = map[Y_HALF][X_HALF + 1]; }
            else if ((typeid(*machine.output) == typeid(Command_Left)) && ('#' != map[Y_HALF][X_HALF - 1])) { --player.x; nsc_x = -TILE; moved = true; nl = map[Y_HALF][X_HALF - 1]; }
            else if (typeid(*machine.output) == typeid(Command_Look))
             {
               Command_Look* temp = dynamic_cast<Command_Look*>(machine.output.get());
               if ((temp->x <= -X_HALF) || (temp->x >= (X_HALF - 1))) machine.input = std::make_shared<Backwards::Types::StringValue>("?");
               else if ((temp->y <= -Y_HALF) || (temp->y >= (Y_HALF - 1))) machine.input = std::make_shared<Backwards::Types::StringValue>("?");
               else
                {
                  switch (map[Y_HALF + temp->y][X_HALF + temp->x])
                   {
                  case '#':
                     machine.input = std::make_shared<Backwards::Types::StringValue>("#");
                     break;
                  case 'D':
                  case 'E':
                  case 'U':
                  case 'V':
                  case 'O':
                     machine.input = std::make_shared<Backwards::Types::StringValue>("D");
                     break;
                  default:
                     machine.input = std::make_shared<Backwards::Types::StringValue>(" ");
                     break;
                   }
                }
             }
          }
         if (true == moved)
          {
            if ((px != player.x) || (py != player.y))
             {
               if ('D' == map[Y_HALF][X_HALF]) --zone.z;
               if ('E' == map[Y_HALF][X_HALF]) zone.z += (true == (mu | (!md & mr))) ? -1 : 1;
               if ('U' == map[Y_HALF][X_HALF]) ++zone.z;
               if ('V' == map[Y_HALF][X_HALF]) zone.z += (true == (mu | (!md & mr))) ? 1 : -1;
             }
            if (('D' == nl) || ('E' == nl) || ('U' == nl) || ('V' == nl))
             {
               px = ppx;
               py = ppy;
             }
            else
             {
               px = 65536;
               py = 65536;
             }

            if (0 == player.x)
             {
               zone.x -= 1;
               player.x += WORLD_WIDTH;
             }
            else if (player.x >= WORLD_WIDTH)
             {
               zone.x += 1;
               player.x -= WORLD_WIDTH;
             }

            if (0 == player.y)
             {
               zone.y -= 1;
               player.y += WORLD_HEIGHT;
             }
            else if (player.y >= WORLD_HEIGHT)
             {
               zone.y += 1;
               player.y -= WORLD_HEIGHT;
             }
          }
         mu = false;
         md = false;
         ml = false;
         mr = false;
       }

      int my = 1;
      for (int y = 0; y < ScreenHeight(); y += TILE)
       {
         int mx = 1;
         for (int x = 0; x < ScreenWidth(); x += TILE)
          {
            switch (map[my][mx])
             {
            case '#':
               DrawSprite(x + sc_x, y + sc_y, floors[getTileNumber(map[my - 1][mx], map[my][mx + 1], map[my + 1][mx], map[my][mx - 1])].get());
               break;
            case 'D':
            case 'E':
            case 'U':
            case 'V':
            case 'O':
               if (' ' != map[my - 1][mx])
                  DrawSprite(x + sc_x, y + sc_y, doors[0].get());
               else
                  DrawSprite(x + sc_x, y + sc_y, doors[1].get());
               break;
            default:
               DrawSprite(x + sc_x, y + sc_y, floors[0].get());
               break;
             }
            ++mx;
          }
         ++my;
       }
      DrawSprite((X_HALF - 1) * TILE, (Y_HALF - 1) * TILE, players.get());
      if (sc_x < 0) ++sc_x;
      else if (sc_x > 0) --sc_x;
      if (sc_y < 0) ++sc_y;
      else if (sc_y > 0) --sc_y;
      if (0 != nsc_x) { sc_x = nsc_x; nsc_x = 0; }
      if (0 != nsc_y) { sc_y = nsc_y; nsc_y = 0; }
      return true;
    }

   void CommandThread()
    {
      for (;;)
       {
         std::string sCommand = logger.get();
         evaluateString(sCommand, context);
       }
    }

	bool OnConsoleCommand(const std::string& sCommand)
    {
      ConsoleOut() << "> " << sCommand << std::endl;

      logger.put(sCommand);

      return true;
    }

private:
   void evaluateString(const std::string& sCommand, Backway::CallingContext& text)
    {
      Backwards::Input::StringInput string (sCommand);
      Backwards::Input::Lexer lexer (string, "Console Command");

      Backwards::Parser::GetterSetter gs;
      Backwards::Parser::SymbolTable table (gs, global);

      std::shared_ptr<Backwards::Engine::Expression> res = Backwards::Parser::Parser::ParseExpression(lexer, table, *text.logger);

      if (nullptr != res.get())
       {

         Backwards::Input::Token token;
         std::shared_ptr<Backwards::Engine::FunctionContext> function = std::make_shared<Backwards::Engine::FunctionContext>();
         function->name = "User Input";
         Backwards::Engine::StackFrame frame (function, token, text.currentFrame);
         text.pushContext(&frame);
         try
          {
            std::shared_ptr<Backwards::Types::ValueType> val = res->evaluate(text);

            if (nullptr != val.get())
             {
               if (typeid(*text.logger) == typeid(ConsoleLogger))
                {
                  Backwards::Engine::DefaultDebugger::printValue(static_cast<ConsoleLogger&>(*text.logger).sink, val);
                  ConsoleOut() << std::endl;
                }
             }
            else
             {
               ConsoleOut() << "Evaluate returned NULL." << std::endl;
             }
          }
         catch (const Backwards::Types::TypedOperationException& e)
          {
            ConsoleOut() << "Caught runtime exception: " << e.what() << std::endl;
          }
         catch (const Backwards::Engine::FatalException& e)
          {
            ConsoleOut() << "Caught Fatal Error: " << e.what() << std::endl;
          }
         catch (const Backwards::Engine::ProgrammingException& e)
          {
            ConsoleOut() << "This is a BUG, please report it: " << e.what() << std::endl;
          }
         catch (...)
          {
            text.popContext();
            throw;
          }
         text.popContext();
       }
      else
       {
         ConsoleOut() << "Parse returned NULL." << std::endl;
       }
    }
 };

int main()
{
   View demo;
   if (demo.Construct(640, 480, 2, 2))
      demo.Start();
   return 0;
}
