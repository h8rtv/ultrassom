template<class ParseResult>
class Parser {
public:
  virtual ParseResult parse() = 0;
};