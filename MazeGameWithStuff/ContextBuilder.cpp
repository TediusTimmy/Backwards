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
#include "Backwards/Input/LineBufferedStreamInput.h"

#include "Backwards/Engine/ConstantsSingleton.h"
#include "Backwards/Engine/Expression.h"
#include "Backwards/Engine/FatalException.h"
#include "Backwards/Engine/ProgrammingException.h"
#include "Backwards/Engine/Logger.h"

#include "Backwards/Parser/ContextBuilder.h"
#include "Backwards/Parser/SymbolTable.h"
#include "Backwards/Parser/Parser.h"

#include "Backwards/Types/FloatValue.h"
#include "Backwards/Types/StringValue.h"
#include "Backwards/Types/DictionaryValue.h"
#include "Backwards/Types/ArrayValue.h"

#include "Backway/ContextBuilder.h"
#include "Backway/CallingContext.h"
#include "Backway/StdLib.h"

#include "Commands.h"

#include <sstream>

STDLIB_CONSTANT_DECL_WITH_CONTEXT(Left)
 {
   try
    {
      Backway::CallingContext& text = dynamic_cast<Backway::CallingContext&>(context);
      text.machine->output = std::make_shared<Command_Left>();
      return Backwards::Engine::ConstantsSingleton::getInstance().FLOAT_ONE;
    }
   catch (const std::bad_cast&)
    {
      throw Backwards::Engine::ProgrammingException("Backwards Context wasn't a Backway Context.");
    }
 }

STDLIB_CONSTANT_DECL_WITH_CONTEXT(Right)
 {
   try
    {
      Backway::CallingContext& text = dynamic_cast<Backway::CallingContext&>(context);
      text.machine->output = std::make_shared<Command_Right>();
      return Backwards::Engine::ConstantsSingleton::getInstance().FLOAT_ONE;
    }
   catch (const std::bad_cast&)
    {
      throw Backwards::Engine::ProgrammingException("Backwards Context wasn't a Backway Context.");
    }
 }

STDLIB_CONSTANT_DECL_WITH_CONTEXT(Up)
 {
   try
    {
      Backway::CallingContext& text = dynamic_cast<Backway::CallingContext&>(context);
      text.machine->output = std::make_shared<Command_Up>();
      return Backwards::Engine::ConstantsSingleton::getInstance().FLOAT_ONE;
    }
   catch (const std::bad_cast&)
    {
      throw Backwards::Engine::ProgrammingException("Backwards Context wasn't a Backway Context.");
    }
 }

STDLIB_CONSTANT_DECL_WITH_CONTEXT(Down)
 {
   try
    {
      Backway::CallingContext& text = dynamic_cast<Backway::CallingContext&>(context);
      text.machine->output = std::make_shared<Command_Down>();
      return Backwards::Engine::ConstantsSingleton::getInstance().FLOAT_ONE;
    }
   catch (const std::bad_cast&)
    {
      throw Backwards::Engine::ProgrammingException("Backwards Context wasn't a Backway Context.");
    }
 }

STDLIB_BINARY_DECL_WITH_CONTEXT(Look)
 {
   try
    {
      Backway::CallingContext& text = dynamic_cast<Backway::CallingContext&>(context);
      if (typeid(Backwards::Types::FloatValue) == typeid(*first))
       {
         if (typeid(Backwards::Types::FloatValue) == typeid(*second))
          {
               // Input Validation? Is that like Water Fluoridation?
            int x = static_cast<int>(static_cast<double>(static_cast<const Backwards::Types::FloatValue&>(*first).value));
            int y = static_cast<int>(static_cast<double>(static_cast<const Backwards::Types::FloatValue&>(*second).value));
            text.machine->output = std::make_shared<Command_Look>(x, y);
          }
         else
          {
            throw Backwards::Types::TypedOperationException("Error looking: y-location not Float.");
          }
       }
      else
       {
         throw Backwards::Types::TypedOperationException("Error looking: x-location not Float.");
       }
      return Backwards::Engine::ConstantsSingleton::getInstance().FLOAT_ONE;
    }
   catch (const std::bad_cast&)
    {
      throw Backwards::Engine::ProgrammingException("Backwards Context wasn't a Backway Context.");
    }
 }

STDLIB_UNARY_DECL_WITH_CONTEXT(Load)
 {
   if (typeid(Backwards::Types::StringValue) == typeid(*arg))
    {
      const std::string& fileName (static_cast<const Backwards::Types::StringValue&>(*arg).value);

      Backwards::Input::FileInput file (fileName);
      Backwards::Input::Lexer lexer (file, fileName);

      Backwards::Parser::GetterSetter gs;
      Backwards::Parser::SymbolTable table (gs, *context.globalScope);

      std::shared_ptr<Backwards::Engine::Expression> res = Backwards::Parser::Parser::ParseExpression(lexer, table, *context.logger);

      if (nullptr != res.get())
       {

         try
          {
            std::shared_ptr<Backwards::Types::ValueType> val = res->evaluate(context);

            if (nullptr != val.get())
             {
               if (typeid(Backwards::Types::DictionaryValue) == typeid(*val))
                {
                  for (std::map<std::shared_ptr<Backwards::Types::ValueType>, std::shared_ptr<Backwards::Types::ValueType>, Backwards::Types::ChristHowHorrifying>::const_iterator iter =
                     static_cast<const Backwards::Types::DictionaryValue&>(*val).value.begin();
                     static_cast<const Backwards::Types::DictionaryValue&>(*val).value.end() != iter; ++iter)
                   {
                     Backway::CreateState(context, iter->first, iter->second);
                   }
                }
               else
                {
                  throw Backwards::Types::TypedOperationException("Load failed: loaded value was not a Dictionary.");
                }
             }
            else
             {
               throw Backwards::Types::TypedOperationException("Load failed: evaluate failed.");
             }
          }
         catch (const Backwards::Types::TypedOperationException& e)
          {
            throw Backwards::Types::TypedOperationException(std::string("Load failed: ") + e.what());
          }
         catch (const Backwards::Engine::FatalException& e)
          {
            throw Backwards::Types::TypedOperationException(std::string("Load failed: ") + e.what());
          }
       }
      else
       {
         throw Backwards::Types::TypedOperationException("Load failed: parse failed.");
       }
    }
   else
    {
      throw Backwards::Types::TypedOperationException("Error loading file: name not String.");
    }
   return Backwards::Engine::ConstantsSingleton::getInstance().FLOAT_ONE;
 }

STDLIB_UNARY_DECL_WITH_CONTEXT(SetInput)
 {
   try
    {
      Backway::CallingContext& text = dynamic_cast<Backway::CallingContext&>(context);
      text.machine->input = arg;
      return Backwards::Engine::ConstantsSingleton::getInstance().FLOAT_ONE;
    }
   catch (const std::bad_cast&)
    {
      throw Backwards::Engine::ProgrammingException("Backwards Context wasn't a Backway Context.");
    }
 }

STDLIB_CONSTANT_DECL_WITH_CONTEXT(DumpMachine)
 {
   try
    {
      Backway::CallingContext& text = dynamic_cast<Backway::CallingContext&>(context);
      std::shared_ptr<Backwards::Types::ArrayValue> result = std::make_shared<Backwards::Types::ArrayValue>();
      std::list<std::string> stateLists;
      for (const auto& iter : text.machine->states)
       {
         std::shared_ptr<Backwards::Types::ArrayValue> temp = std::make_shared<Backwards::Types::ArrayValue>();
         std::ostringstream layer;
         layer << "-";
         for (const auto& state : iter)
          {
            temp->value.push_back(std::make_shared<Backwards::Types::StringValue>(state->scope.name));
            layer << " " << state->scope.name;
          }
         stateLists.push_front(layer.str());
         result->value.push_back(temp);
       }
      for (const auto& list : stateLists)
       {
         text.logger->log(list);
       }
      return result;
    }
   catch (const std::bad_cast&)
    {
      throw Backwards::Engine::ProgrammingException("Backwards Context wasn't a Backway Context.");
    }
 }

   // This is copy-pasted from main.cpp. This is bad practice.
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
extern DebuggerLogger* nastyHack;

   // This is STDLIB_UNARY_DECL expanded, because it was never intended to be used outside of Backwards.
std::shared_ptr<Backwards::Types::ValueType> SetDebugScript (const std::shared_ptr<Backwards::Types::ValueType>& arg)
 {
   if (typeid(Backwards::Types::ArrayValue) == typeid(*arg))
    {
      std::vector<std::string> commands;
      for (const auto& ptr : static_cast<const Backwards::Types::ArrayValue&>(*arg).value)
       {
         if (typeid(Backwards::Types::StringValue) == typeid(*ptr))
          {
            commands.push_back(static_cast<const Backwards::Types::StringValue&>(*ptr).value);
          }
         else
          {
            throw Backwards::Types::TypedOperationException("Error setting script: not an Array of String.");
          }
       }
      if (nullptr != nastyHack)
       {
         nastyHack->commands = commands;
         nastyHack->command = 0U;
       }
      else
       {
         throw Backwards::Engine::ProgrammingException("Where's the logger?");
       }
    }
   else
    {
      throw Backwards::Types::TypedOperationException("Error setting script: not Array.");
    }
   return Backwards::Engine::ConstantsSingleton::getInstance().FLOAT_ONE;
 }

class ContextBuilder final
 {
public:
   static void createGlobalScope(Backwards::Engine::Scope&);
 };

void ContextBuilder::createGlobalScope (Backwards::Engine::Scope& global)
 {
   Backway::ContextBuilder::createGlobalScope(global);

   Backwards::Parser::ContextBuilder::addFunction("Left", std::make_shared<Backwards::Engine::StandardConstantFunctionWithContext>(Left), 0U, global);
   Backwards::Parser::ContextBuilder::addFunction("Right", std::make_shared<Backwards::Engine::StandardConstantFunctionWithContext>(Right), 0U, global);
   Backwards::Parser::ContextBuilder::addFunction("Up", std::make_shared<Backwards::Engine::StandardConstantFunctionWithContext>(Up), 0U, global);
   Backwards::Parser::ContextBuilder::addFunction("Down", std::make_shared<Backwards::Engine::StandardConstantFunctionWithContext>(Down), 0U, global);
   Backwards::Parser::ContextBuilder::addFunction("DumpMachine", std::make_shared<Backwards::Engine::StandardConstantFunctionWithContext>(DumpMachine), 0U, global);

   Backwards::Parser::ContextBuilder::addFunction("Load", std::make_shared<Backwards::Engine::StandardUnaryFunctionWithContext>(Load), 1U, global);
   Backwards::Parser::ContextBuilder::addFunction("SetInput", std::make_shared<Backwards::Engine::StandardUnaryFunctionWithContext>(SetInput), 1U, global);
   Backwards::Parser::ContextBuilder::addFunction("SetDebugScript", std::make_shared<Backwards::Engine::StandardUnaryFunction>(SetDebugScript), 1U, global);

   Backwards::Parser::ContextBuilder::addFunction("Look", std::make_shared<Backway::StandardBinaryFunctionWithContext>(Look), 2U, global);
 }
