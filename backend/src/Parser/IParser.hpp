#pragma once 

template<class ParseResult>
class IParser {
public:
  virtual ParseResult parse() = 0;
};