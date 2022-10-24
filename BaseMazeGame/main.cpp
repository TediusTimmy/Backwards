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
#include <memory>
#include <vector>

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
   View()
    {
      sAppName = "Backroom Quest Alpha";
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

      return true;
    }

   char getMapTile(const int tx, const int ty, const unsigned int z)
    {
		if (tx < 0)
         return getCache(zone.x - 1, zone.y, z)[ty * WORLD_WIDTH + tx + WORLD_WIDTH];
		else if (tx >= WORLD_WIDTH)
         return getCache(zone.x + 1, zone.y, z)[ty * WORLD_WIDTH + tx - WORLD_WIDTH];
		else if (ty < 0)
         return getCache(zone.x, zone.y - 1, z)[(ty + WORLD_HEIGHT) * WORLD_WIDTH + tx];
		else if (ty >= WORLD_HEIGHT)
         return getCache(zone.x, zone.y + 1, z)[(ty - WORLD_HEIGHT) * WORLD_WIDTH + tx];
      else
			return getCache(zone.x, zone.y, z)[ty * WORLD_WIDTH + tx];
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
         if ((true == mu) && ('#' != map[Y_HALF - 1][X_HALF])) { --player.y; nsc_y = -TILE; moved = true; nl = map[Y_HALF - 1][X_HALF]; }
         else if ((true == md) && ('#' != map[Y_HALF + 1][X_HALF])) { ++player.y; nsc_y = TILE; moved = true; nl = map[Y_HALF + 1][X_HALF]; }
         else if ((true == mr) && ('#' != map[Y_HALF][X_HALF + 1])) { ++player.x; nsc_x = TILE; moved = true; nl = map[Y_HALF][X_HALF + 1]; }
         else if ((true == ml) && ('#' != map[Y_HALF][X_HALF - 1])) { --player.x; nsc_x = -TILE; moved = true; nl = map[Y_HALF][X_HALF - 1]; }
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
 };

int main()
{
   View demo;
   if (demo.Construct(640, 480, 2, 2))
      demo.Start();
   return 0;
}
