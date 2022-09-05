/*
   No rights applicable.
*/

/*
   This is the 48-bit LCG used by the Java programming language
   for its default random number generator.
*/

class JAVA
 {
   private:
      uint64_t seed;

   public:
      JAVA () : seed(0U) { }
      JAVA (uint64_t seed) : seed(seed & 0xFFFFFFFFFFFFULL) { }

      JAVA (const JAVA& src) = default;
      JAVA& operator= (const JAVA&) = default;

      uint64_t getNext()
       {
         seed = (25214903917ULL * seed + 11) & 0xFFFFFFFFFFFFULL;
         return seed;
       }
 };
