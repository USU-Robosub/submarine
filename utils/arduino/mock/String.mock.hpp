#ifndef STRING_MOCK
#define STRING_MOCK

class String{
public:
  String(const char* cstr) : cstr((char*)cstr){

  }

  bool equals(const char* other){
    bool null = false, same = true;
    int index = 0;
    while(!null && same){
      if(this->cstr[index] != other[index]){
        same = false;
      }
      char next = this->cstr[index];
      if(next == 0){
        null = true;
      }
      next = other[index++];
      if(next == 0){
        null = true;
      }
    }
    return same;
  }

private:
  char* cstr;
};

#endif
