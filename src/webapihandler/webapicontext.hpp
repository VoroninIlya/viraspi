#pragma once

#include "mongoose.h"

#include <string>
#include <vector>
#include <utility>

class IWebApiContext {
public:

  IWebApiContext() : con(NULL) {}

  IWebApiContext(mg_connection* con) : con(con){}

  IWebApiContext(const IWebApiContext& obj) {
    swap(obj);
  }

  IWebApiContext(IWebApiContext&& obj) {
    swap(obj);
  }

  IWebApiContext& operator=(const IWebApiContext& obj) {
    if(this != &obj) {
      swap(obj);
    }
    return *this;
  }

  IWebApiContext& operator=(IWebApiContext&& obj) noexcept {
    if(this != &obj) {
      swap(obj);
    }
    return *this;
  }

  mg_connection* con;

private:
  void swap(const IWebApiContext& obj) noexcept {
    this->con = obj.con;
  }

};