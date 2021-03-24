// std::memcpy

// Defined in header <cstring>
// void* memcpy( void* dest, const void* src, std::size_t count );

// copies count bytes from the object pointed to by src
// to the object pointed to by dest. Both objects are reinterpreted
// as arrays of unsigned char.

// If the objects overlap, the behavior is undefined.
#include <iostream>
#include <cstdint>
#include <cstring>

void *memcpy2(void *dest, const void *src, size_t count)
{
  if (dest == nullptr || src == nullptr)
    return nullptr;
  
  char *psrc = nullptr, *pdest = nullptr;
  
  // if overlap
  if (src < dest && (char *)src + count > dest)
    {
      psrc = (char *)src + count - 1;
      pdest = (char *)dest + count - 1;
      while (count--)
	{
	  *pdest-- = *psrc--;
	}
    }
  else
    {
      psrc = (char *)src;
      pdest = (char *)dest;
      while (count--)
	{
	  *pdest++ = *psrc++;
	}
    }
  
  return dest;
}

int main(int argc, char *argv[])
{
  // simple usage
  char source[] = "once upon a midnight dreary...", dest[4];
  memcpy2(dest, source, sizeof dest);
  std::cout << "dest[4] = { ";
  for (char c : dest)
    std::cout << "'" << c << "', ";
  std::cout << "};\n";
 
  // reinterpreting
  double d = 0.1;
  //  std::int64_t n = *reinterpret_cast<std::int64_t*>(&d); // aliasing violation
  std::int64_t n;
  memcpy2(&n, &d, sizeof d); // OK
 
  std::cout << std::hexfloat << d << " is " << std::hex << n
	    << " as an std::int64_t\n" << std::dec;
 
  // object creation in destination buffer
  struct S {
    int x{42};
    void print() const { std::cout << "{" << x << "}\n"; }
  } s;
  alignas(S) char buf[sizeof(S)];
  S* ps = new (buf) S; // placement new
  ps->print();
  memcpy2(ps, &s, sizeof s);
  ps->print();
  
  return 0;
}
