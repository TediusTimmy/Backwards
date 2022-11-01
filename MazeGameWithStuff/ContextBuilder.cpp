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
#include "Backwards/Engine/ConstantsSingleton.h"
#include "Backwards/Engine/ProgrammingException.h"
#include "Backwards/Parser/ContextBuilder.h"

#include "Backway/ContextBuilder.h"
#include "Backway/CallingContext.h"
#include "Backway/StdLib.h"

#include "Commands.h"

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
 }
