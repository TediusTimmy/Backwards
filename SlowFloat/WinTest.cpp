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
#include <iostream>
#include <iomanip>
#include <cmath>
#include "SlowFloat.h"

/*
const int16_t CUTOFF = 9;

void showProgress(double arg)
 {
   int16_t exponent = static_cast<int16_t>(std::floor(std::log(std::fabs(arg)) / std::log(10.0)));
   std::cout << exponent <<std::endl;
   uint32_t significand = static_cast<uint32_t>(std::fabs(arg) / std::pow(10.0, exponent) * 100000000);
   std::cout << significand <<std::endl;
   double temp = 5.0;
   if ((exponent - CUTOFF + 1) >= 0)
      temp -= (std::fabs(arg) / std::pow(10.0, exponent - CUTOFF + 1) - significand) * 10;
   else
      temp -= (std::fabs(arg) * std::pow(10.0, -(exponent - CUTOFF + 1)) - significand) * 10;

      std::cout << std::scientific << std::setprecision(19);
      std::cout << temp << std::endl;
      std::cout << std::fabs(arg) << std::endl;
      std::cout << std::pow(10.0, exponent - CUTOFF + 1) << std::endl;
      std::cout << (std::fabs(arg) / std::pow(10.0, exponent - CUTOFF + 1)) << std::endl;
      std::cout << (std::fabs(arg) / std::pow(10.0, exponent - CUTOFF + 1) - significand) << std::endl;
      std::cout << std::exp((exponent - CUTOFF + 1) * std::log(10.0)) << std::endl;
      std::cout << (std::fabs(arg) / std::exp((exponent - CUTOFF + 1) * std::log(10.0))) << std::endl;
      std::cout << (std::fabs(arg) / std::exp((exponent - CUTOFF + 1) * std::log(10.0)) - significand) << std::endl;
 }

int main (void)
 {
    {
      showProgress(2.0);
    }
   SlowFloat::mode = SlowFloat::ROUND_POSITIVE_INFINITY;
    {
      SlowFloat::SlowFloat bob (2.0);
      std::cout << bob.significand << " " << bob.exponent << std::endl;
    }
   return 0;
 }
*/

int main (void)
 {
   SlowFloat::SlowFloat bob (7.0);
   std::cout << bob.significand << " " << bob.exponent << std::endl;
   std::cout << std::scientific << std::setprecision(19) << static_cast<double>(bob) << std::endl;
   return 0;
 }