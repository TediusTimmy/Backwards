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
#ifndef BACKWARDS_INPUT_LEXEMES_H
#define BACKWARDS_INPUT_LEXEMES_H

namespace Backwards
 {

namespace Input
 {

   enum Lexeme
    {
      INVALID,
      END_OF_FILE,
      LEXER_NEVER_RETURNS_THIS,

      FUNCTION,
      SEMICOLON,
      END,

      IDENTIFIER,
      NUMBER,
      STRING,

      OPEN_PARENS,
      CLOSE_PARENS,
      OPEN_BRACKET,
      CLOSE_BRACKET,
      OPEN_BRACE,
      CLOSE_BRACE,
      PERIOD,

      SET,
      TO,
      CALL,
      IF,
      THEN,
      ELSE,
      ELSEIF,
      WHILE,
      DO,
      SELECT,
      FROM,
      CASE,
      IS,
      ALSO,
      ABOVE,
      BELOW,
      BREAK,
      CONTINUE,
      RETURN,
      FOR,
      DOWNTO,
      STEP,
      IN,

      EQUALITY,
      INEQUALITY,
      CONDITIONAL,
      ALTERNATIVE,
      SHORT_AND,
      SHORT_OR,
      GREATER_THAN,
      LESS_THAN,
      GREATER_THAN_OR_EQUAL_TO,
      LESS_THAN_OR_EQUAL_TO,
      PLUS,
      MINUS,
      MULTIPLY,
      DIVIDE,
      POWER,
      NOT
    };

 } // namespace Input

 } // namespace Backwards

#endif /* BACKWARDS_INPUT_LEXEMES_H */
