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
#include "gtest/gtest.h"

#include "Backwards/Engine/StdLib.h"

#include "Backwards/Engine/FatalException.h"
#include "Backwards/Engine/CallingContext.h"
#include "Backwards/Engine/Logger.h"
#include "Backwards/Engine/DebuggerHook.h"

#include "Backwards/Types/DoubleValue.h"
#include "Backwards/Types/StringValue.h"
#include "Backwards/Types/ArrayValue.h"
#include "Backwards/Types/DictionaryValue.h"
#include "Backwards/Types/FunctionValue.h" // One function uses it!

class StringLogger final : public Backwards::Engine::Logger
 {
public:
   std::vector<std::string> logs;
   void log (const std::string& message) { logs.emplace_back(message); }
   std::string get () { return ""; }
 };

static std::shared_ptr<Backwards::Types::DoubleValue> makeDoubleValue (double d = 0.0)
 {
   return std::make_shared<Backwards::Types::DoubleValue>(SlowFloat::SlowFloat(d));
 }

TEST(EngineTests, testFunctionsWithContext)
 {
   std::shared_ptr<Backwards::Types::ValueType> res;
   Backwards::Engine::CallingContext context;
   StringLogger logger;
   context.logger = &logger;

   res = Backwards::Engine::Info(context, std::make_shared<Backwards::Types::StringValue>("hello"));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("hello", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);

   ASSERT_EQ(1U, logger.logs.size());
   EXPECT_EQ("INFO: hello", logger.logs[0]);
   logger.logs.clear();

   res = Backwards::Engine::Warn(context, std::make_shared<Backwards::Types::StringValue>("hello"));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("hello", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);

   ASSERT_EQ(1U, logger.logs.size());
   EXPECT_EQ("WARN: hello", logger.logs[0]);
   logger.logs.clear();

   res = Backwards::Engine::Error(context, std::make_shared<Backwards::Types::StringValue>("hello"));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("hello", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);

   ASSERT_EQ(1U, logger.logs.size());
   EXPECT_EQ("ERROR: hello", logger.logs[0]);
   logger.logs.clear();

   res = Backwards::Engine::DebugPrint(context, std::make_shared<Backwards::Types::StringValue>("hello"));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("hello", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);

   ASSERT_EQ(1U, logger.logs.size());
   EXPECT_EQ("hello", logger.logs[0]);
   logger.logs.clear();

   try
    {
      (void) Backwards::Engine::Fatal(context, std::make_shared<Backwards::Types::StringValue>("hello"));
      FAIL() << "Didn't throw exception.";
    }
   catch(Backwards::Engine::FatalException& e)
    {
      EXPECT_STREQ("hello", e.what());
    }
   ASSERT_EQ(1U, logger.logs.size());
   EXPECT_EQ("FATAL: hello", logger.logs[0]);
   logger.logs.clear();

   EXPECT_THROW(Backwards::Engine::Info(context, makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::Warn(context, makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::Error(context, makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::DebugPrint(context, makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);
      // Fatal still throws a FatalException when given bad input.
   EXPECT_THROW(Backwards::Engine::Fatal(context, makeDoubleValue(1.0)), Backwards::Engine::FatalException);
 }

TEST(EngineTests, testBasicEight) // The seven functions needed for the parser and ToString.
 {
   std::shared_ptr<Backwards::Types::ValueType> res, left, right;

   res = Backwards::Engine::NewArray();
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*res.get()));
   EXPECT_EQ(0U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(res)->value.size());

   res = Backwards::Engine::NewDictionary();
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*res.get()));
   EXPECT_EQ(0U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(res)->value.size());

      // Result has dependencies below.
   res = Backwards::Engine::PushBack(Backwards::Engine::NewArray(), std::make_shared<Backwards::Types::StringValue>("hello"));
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*res.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(res)->value.size());
   left = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(res)->value[0];
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*left.get()));
   EXPECT_EQ("hello", std::dynamic_pointer_cast<Backwards::Types::StringValue>(left)->value);

   EXPECT_THROW(Backwards::Engine::PushBack(std::make_shared<Backwards::Types::StringValue>("hello"), Backwards::Engine::NewArray()), Backwards::Types::TypedOperationException);

      // Depend on res above.
   EXPECT_THROW(Backwards::Engine::GetIndex(res, makeDoubleValue(-3.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::GetIndex(res, makeDoubleValue(-1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::GetIndex(res, makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::GetIndex(res, makeDoubleValue(3.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::GetIndex(res, std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);

      // Depends on res above.
   left = Backwards::Engine::GetIndex(res, makeDoubleValue(0.0));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*left.get()));
   EXPECT_EQ("hello", std::dynamic_pointer_cast<Backwards::Types::StringValue>(left)->value);

   EXPECT_THROW(Backwards::Engine::GetIndex(Backwards::Engine::NewDictionary(), makeDoubleValue(0.0)), Backwards::Types::TypedOperationException);

      // Depend on res above.
   EXPECT_THROW(Backwards::Engine::SetIndex(res, makeDoubleValue(-3.0), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::SetIndex(res, makeDoubleValue(-1.0), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::SetIndex(res, makeDoubleValue(1.0), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::SetIndex(res, makeDoubleValue(3.0), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::SetIndex(res, std::make_shared<Backwards::Types::StringValue>("world"), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);

      // Depends on res above.
   right = Backwards::Engine::SetIndex(res, makeDoubleValue(0.0), std::make_shared<Backwards::Types::StringValue>("world"));
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*right.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(right)->value.size());
   left = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(right)->value[0];
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*left.get()));
   EXPECT_EQ("world", std::dynamic_pointer_cast<Backwards::Types::StringValue>(left)->value);

   EXPECT_THROW(Backwards::Engine::SetIndex(Backwards::Engine::NewDictionary(), makeDoubleValue(0.0), makeDoubleValue(0.0)), Backwards::Types::TypedOperationException);

      // Result has dependencies below.
   res = Backwards::Engine::Insert(Backwards::Engine::NewDictionary(), std::make_shared<Backwards::Types::StringValue>("hello"), std::make_shared<Backwards::Types::StringValue>("world"));
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*res.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(res)->value.size());
   left = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(res)->value.begin()->first;
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*left.get()));
   EXPECT_EQ("hello", std::dynamic_pointer_cast<Backwards::Types::StringValue>(left)->value);
   right = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(res)->value.begin()->second;
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*right.get()));
   EXPECT_EQ("world", std::dynamic_pointer_cast<Backwards::Types::StringValue>(right)->value);

   EXPECT_THROW(Backwards::Engine::Insert(Backwards::Engine::NewArray(), std::make_shared<Backwards::Types::StringValue>("hello"), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);

      // Depends on res above.
   EXPECT_THROW(Backwards::Engine::GetValue(res, std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);

      // Depends on res above.
   res = Backwards::Engine::GetValue(res, std::make_shared<Backwards::Types::StringValue>("hello"));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("world", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);

   EXPECT_THROW(Backwards::Engine::GetValue(Backwards::Engine::NewArray(), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);

   res = Backwards::Engine::ToString(makeDoubleValue(314.0));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("3.14000000e+2", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);

   EXPECT_THROW(Backwards::Engine::ToString(std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);
 }

TEST(EngineTests, testJustCalls) // Don't bother validating that the output is correct.
 {
   std::shared_ptr<Backwards::Types::ValueType> res; // I KNOW WHAT I JUST SAID!
   res = Backwards::Engine::PI();
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(3.14159265358979323846264338327950288419716939937510582097494), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::Date();
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ(10U, std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value.length());

   res = Backwards::Engine::Time();
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ(8U, std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value.length());

   (void) Backwards::Engine::Atan2(makeDoubleValue(1.0), makeDoubleValue(1.0));
   EXPECT_THROW(Backwards::Engine::Atan2(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::Atan2(makeDoubleValue(1.0), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Hypot(makeDoubleValue(1.0), makeDoubleValue(1.0));
   EXPECT_THROW(Backwards::Engine::Hypot(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::Hypot(makeDoubleValue(1.0), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);

      // Check for the bug found while converting to SlowFloat.
   res = Backwards::Engine::Hypot(makeDoubleValue(3.0), makeDoubleValue(4.0));
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(5.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   (void) Backwards::Engine::Log(makeDoubleValue(1.0), makeDoubleValue(1.0));
   EXPECT_THROW(Backwards::Engine::Log(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::Log(makeDoubleValue(1.0), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);

   res = Backwards::Engine::Sin(makeDoubleValue(30.0)); // Make sure conversion is applied.
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(0.5), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);
   EXPECT_THROW(Backwards::Engine::Sin(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Cos(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Cos(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Tan(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Tan(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   res = Backwards::Engine::Asin(makeDoubleValue(0.5)); // Make sure conversion is applied.
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(30.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);
   EXPECT_THROW(Backwards::Engine::Asin(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Acos(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Acos(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Atan(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Atan(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Exp(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Exp(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Ln(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Ln(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Sqrt(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Sqrt(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Cbrt(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Cbrt(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Abs(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Abs(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Round(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Round(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Floor(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Floor(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Ceil(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Ceil(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::IsInfinity(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::IsInfinity(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::IsNaN(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::IsNaN(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Sinh(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Sinh(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Cosh(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Cosh(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Tanh(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Tanh(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::Sqr(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::Sqr(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

      // Assume that if Sin is right, then DTR is right and RTD must be.
   (void) Backwards::Engine::DegToRad(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::DegToRad(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   (void) Backwards::Engine::RadToDeg(makeDoubleValue(30.0));
   EXPECT_THROW(Backwards::Engine::RadToDeg(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);
 }

TEST(EngineTests, testSimpleCalls)
 {
   std::shared_ptr<Backwards::Types::ValueType> res;

   res = Backwards::Engine::IsDouble(makeDoubleValue());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(1.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::IsDouble(std::make_shared<Backwards::Types::StringValue>());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(0.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::IsString(std::make_shared<Backwards::Types::StringValue>());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(1.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::IsString(makeDoubleValue());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(0.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::IsArray(std::make_shared<Backwards::Types::ArrayValue>());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(1.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::IsArray(std::make_shared<Backwards::Types::StringValue>());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(0.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::IsDictionary(std::make_shared<Backwards::Types::DictionaryValue>());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(1.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::IsDictionary(std::make_shared<Backwards::Types::StringValue>());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(0.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::IsFunction(std::make_shared<Backwards::Types::FunctionValue>());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(1.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::IsFunction(std::make_shared<Backwards::Types::StringValue>());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(0.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::Length(std::make_shared<Backwards::Types::StringValue>("hello"));
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(5.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   EXPECT_THROW(Backwards::Engine::Length(makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);

   res = Backwards::Engine::Size(Backwards::Engine::NewArray());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(0.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   res = Backwards::Engine::Size(Backwards::Engine::NewDictionary());
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(0.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   EXPECT_THROW(Backwards::Engine::Size(makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);

   std::shared_ptr<Backwards::Types::DoubleValue> one = makeDoubleValue(6.0);
   std::shared_ptr<Backwards::Types::DoubleValue> two = makeDoubleValue(9.0);
   std::shared_ptr<Backwards::Types::ValueType> nan = Backwards::Engine::Asin(makeDoubleValue(5.0));

   res = Backwards::Engine::Min(one, two);
   EXPECT_EQ(one.get(), res.get()); // Yes : compare the POINTERS.
   res = Backwards::Engine::Min(two, one);
   EXPECT_EQ(one.get(), res.get());
   res = Backwards::Engine::Min(nan, one);
   EXPECT_EQ(nan.get(), res.get());
   res = Backwards::Engine::Min(one, nan);
   EXPECT_EQ(nan.get(), res.get());
   EXPECT_THROW(Backwards::Engine::Min(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::Min(makeDoubleValue(1.0), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);

   res = Backwards::Engine::Max(one, two);
   EXPECT_EQ(two.get(), res.get());
   res = Backwards::Engine::Max(two, one);
   EXPECT_EQ(two.get(), res.get());
   res = Backwards::Engine::Max(nan, one);
   EXPECT_EQ(nan.get(), res.get());
   res = Backwards::Engine::Max(one, nan);
   EXPECT_EQ(nan.get(), res.get());
   EXPECT_THROW(Backwards::Engine::Max(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::Max(makeDoubleValue(1.0), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);

   res = Backwards::Engine::ValueOf(std::make_shared<Backwards::Types::StringValue>("12"));
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(12.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   EXPECT_THROW(Backwards::Engine::ValueOf(std::make_shared<Backwards::Types::StringValue>("12h")), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::ValueOf(std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::ValueOf(makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);

   res = Backwards::Engine::FromCharacter(std::make_shared<Backwards::Types::StringValue>("H"));
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(72.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);

   EXPECT_THROW(Backwards::Engine::FromCharacter(std::make_shared<Backwards::Types::StringValue>("Hi")), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::FromCharacter(makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);

   res = Backwards::Engine::ToCharacter(makeDoubleValue(72.0));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("H", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);

   EXPECT_THROW(Backwards::Engine::ToCharacter(makeDoubleValue(-1024.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::ToCharacter(makeDoubleValue(512.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::ToCharacter(std::make_shared<Backwards::Types::StringValue>("Hi")), Backwards::Types::TypedOperationException);
 }

TEST(EngineTests, testArrayFunctions)
 {
   std::shared_ptr<Backwards::Types::ValueType> res, left;

      // Result has dependencies below.
   res = Backwards::Engine::PushFront(Backwards::Engine::PushBack(Backwards::Engine::NewArray(), std::make_shared<Backwards::Types::StringValue>("world")), std::make_shared<Backwards::Types::StringValue>("hello"));
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*res.get()));
   ASSERT_EQ(2U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(res)->value.size());
   left = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(res)->value[0];
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*left.get()));
   EXPECT_EQ("hello", std::dynamic_pointer_cast<Backwards::Types::StringValue>(left)->value);
   left = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(res)->value[1];
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*left.get()));
   EXPECT_EQ("world", std::dynamic_pointer_cast<Backwards::Types::StringValue>(left)->value);

      // Depend on res above.
   left = Backwards::Engine::PopFront(res);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*left.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(left)->value.size());
   left = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(left)->value[0];
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*left.get()));
   EXPECT_EQ("world", std::dynamic_pointer_cast<Backwards::Types::StringValue>(left)->value);

      // Depend on res above.
   left = Backwards::Engine::PopBack(res);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*left.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(left)->value.size());
   left = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(left)->value[0];
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*left.get()));
   EXPECT_EQ("hello", std::dynamic_pointer_cast<Backwards::Types::StringValue>(left)->value);

   EXPECT_THROW(Backwards::Engine::PushFront(Backwards::Engine::NewDictionary(), Backwards::Engine::NewArray()), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::PopFront(Backwards::Engine::NewDictionary()), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::PopBack(Backwards::Engine::NewDictionary()), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::PopFront(Backwards::Engine::NewArray()), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::PopBack(Backwards::Engine::NewArray()), Backwards::Types::TypedOperationException);

   res = Backwards::Engine::NewArrayDefault(makeDoubleValue(2.0), std::make_shared<Backwards::Types::StringValue>("world"));
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*res.get()));
   ASSERT_EQ(2U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(res)->value.size());
   left = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(res)->value[0];
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*left.get()));
   EXPECT_EQ("world", std::dynamic_pointer_cast<Backwards::Types::StringValue>(left)->value);
   left = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(res)->value[1];
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*left.get()));
   EXPECT_EQ("world", std::dynamic_pointer_cast<Backwards::Types::StringValue>(left)->value);

   EXPECT_THROW(Backwards::Engine::NewArrayDefault(std::make_shared<Backwards::Types::StringValue>("world"), makeDoubleValue(2.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::NewArrayDefault(makeDoubleValue(-1.0), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::NewArrayDefault(makeDoubleValue(1e100), std::make_shared<Backwards::Types::StringValue>("world")), Backwards::Types::TypedOperationException);
 }

TEST(EngineTests, testDictionaryFunctions)
 {
   std::shared_ptr<Backwards::Types::ValueType> res, left, right;
   res = Backwards::Engine::Insert(Backwards::Engine::NewDictionary(), std::make_shared<Backwards::Types::StringValue>("hello"), std::make_shared<Backwards::Types::StringValue>("world"));
   res = Backwards::Engine::Insert(res, std::make_shared<Backwards::Types::StringValue>("world"), std::make_shared<Backwards::Types::StringValue>("hello"));
      // Result has dependencies below.
   res = Backwards::Engine::Insert(res, std::make_shared<Backwards::Types::StringValue>("world"), std::make_shared<Backwards::Types::StringValue>("hello"));
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*res.get()));
   ASSERT_EQ(2U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(res)->value.size());

   left = Backwards::Engine::ContainsKey(res, std::make_shared<Backwards::Types::StringValue>("hello"));
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*left.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(1.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(left)->value);

   left = Backwards::Engine::ContainsKey(res, std::make_shared<Backwards::Types::StringValue>("shenanigans"));
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*left.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(0.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(left)->value);

   EXPECT_THROW(Backwards::Engine::ContainsKey(Backwards::Engine::NewArray(), std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   left = Backwards::Engine::RemoveKey(res, std::make_shared<Backwards::Types::StringValue>("hello"));
   ASSERT_TRUE(typeid(Backwards::Types::DictionaryValue) == typeid(*left.get()));
   ASSERT_EQ(1U, std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(left)->value.size());
   right = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(left)->value.begin()->first;
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*right.get()));
   EXPECT_EQ("world", std::dynamic_pointer_cast<Backwards::Types::StringValue>(right)->value);
   right = std::dynamic_pointer_cast<Backwards::Types::DictionaryValue>(left)->value.begin()->second;
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*right.get()));
   EXPECT_EQ("hello", std::dynamic_pointer_cast<Backwards::Types::StringValue>(right)->value);

   EXPECT_THROW(Backwards::Engine::RemoveKey(Backwards::Engine::NewArray(), std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::RemoveKey(res, std::make_shared<Backwards::Types::StringValue>("shenanigans")), Backwards::Types::TypedOperationException);

   left = Backwards::Engine::GetKeys(res);
   ASSERT_TRUE(typeid(Backwards::Types::ArrayValue) == typeid(*left.get()));
   ASSERT_EQ(2U, std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(left)->value.size());
   right = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(left)->value[0];
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*right.get()));
   EXPECT_EQ("hello", std::dynamic_pointer_cast<Backwards::Types::StringValue>(right)->value);
   right = std::dynamic_pointer_cast<Backwards::Types::ArrayValue>(left)->value[1];
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*right.get()));
   EXPECT_EQ("world", std::dynamic_pointer_cast<Backwards::Types::StringValue>(right)->value);

   EXPECT_THROW(Backwards::Engine::GetKeys(Backwards::Engine::NewArray()), Backwards::Types::TypedOperationException);
 }

TEST(EngineTests, testSubstring)
 {
   EXPECT_THROW(Backwards::Engine::SubString(Backwards::Engine::NewArray(), makeDoubleValue(1.0), makeDoubleValue(2.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(-1.0), makeDoubleValue(2.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(1.0), makeDoubleValue(8.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(8.0), makeDoubleValue(10.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(2.0), makeDoubleValue(1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(2.0), makeDoubleValue(-1.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(2.0)), Backwards::Types::TypedOperationException);
   EXPECT_THROW(Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(-1.0), std::make_shared<Backwards::Types::StringValue>("hello")), Backwards::Types::TypedOperationException);

   std::shared_ptr<Backwards::Types::ValueType> res;

   res = Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(1.0), makeDoubleValue(1.0));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ(0U, std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value.length());

   res = Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(0.0), makeDoubleValue(1.0));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("h", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);

   res = Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(0.0), makeDoubleValue(2.0));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("he", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);

   res = Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(4.0), makeDoubleValue(5.0));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("o", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);

   res = Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(3.0), makeDoubleValue(5.0));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("lo", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);

   res = Backwards::Engine::SubString(std::make_shared<Backwards::Types::StringValue>("hello"), makeDoubleValue(1.0), makeDoubleValue(4.0));
   ASSERT_TRUE(typeid(Backwards::Types::StringValue) == typeid(*res.get()));
   EXPECT_EQ("ell", std::dynamic_pointer_cast<Backwards::Types::StringValue>(res)->value);
 }

class DummyDebugger final : public Backwards::Engine::DebuggerHook
 {
public:
   bool entered;
   virtual void EnterDebugger(const std::string&, Backwards::Engine::CallingContext&) { entered = true; }
 };

TEST(EngineTests, testEnterDebugger)
 {
   std::shared_ptr<Backwards::Types::ValueType> res;
   Backwards::Engine::CallingContext context;
   StringLogger logger;
   context.logger = &logger;
   DummyDebugger debugger;
   context.debugger = &debugger;

   res = Backwards::Engine::EnterDebugger(context);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(0.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);
   EXPECT_TRUE(debugger.entered);
   EXPECT_EQ(0U, logger.logs.size());

   context.debugger = nullptr;

   res = Backwards::Engine::EnterDebugger(context);
   ASSERT_TRUE(typeid(Backwards::Types::DoubleValue) == typeid(*res.get()));
   EXPECT_EQ(SlowFloat::SlowFloat(0.0), std::dynamic_pointer_cast<Backwards::Types::DoubleValue>(res)->value);
   EXPECT_EQ(0U, logger.logs.size());
 }
