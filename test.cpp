#include <string>
#include <vector>
#include <map>
#include <iostream>

class Json
  {
  public:
    class Value;

    enum ValueType {
      arrayValue,
      objectValue,
    };

    typedef size_t ArrayIndex;

  private:
    class ObjectValue;
    class ArrayValue;

    class Base
    {
    public:
      ValueType type() const
      {
        return type_;
      }

    protected:
      Base(const ValueType type)
        : type_(type)
      {
      }

      virtual ~Base() {}

    private:
      friend class Value;

      const ValueType type_;
    };

  public:
    class Value
    {
    public:
      Value()
        : px(nullptr)
      {
      }

      explicit Value(const ValueType type)
        : px(new_value(type))
      {
      }

      ~Value()
      {
        if (px)
          delete px;
      }

      ValueType type() const
      {
        return value()->type();
      }

      // Return true if empty array, empty object, empty string, or null;
      // otherwise, false.
      bool empty() const
      {
        const Base* v = value();
        switch (v->type())
          {
          case arrayValue:
            return static_cast<const ArrayValue*>(v)->empty();
          case objectValue:
            return static_cast<const ObjectValue*>(v)->empty();
          default:
            return false;
          }
      }

      // Reserve size in array
      void reserve(const ArrayIndex size)
      {
        Base* v = value();
        switch (v->type())
          {
          case arrayValue:
            static_cast<ArrayValue*>(v)->reserve(size);
            break;
          }
      }

    private:
      Value(Base* base)
        : px(base)
      {
      }

      const Base* value() const
      {
        return px;
      }

      Base* value()
      {
        return px;
      }

      static Base* new_value(const ValueType type)
      {
        switch (type)
          {
          case arrayValue:
            return new ArrayValue();
          case objectValue:
            return new ObjectValue();
          }
        return nullptr;
      }

      Base* px;
    };

  private:
    class ArrayValue : public Base
    {
    public:
      ArrayValue()
        : Base(arrayValue)
      {
      }

      bool empty() const
      {
        return array.empty();
      }

      void reserve(const ArrayIndex size)
      {
        array.reserve(size);
      }

      std::vector<Value> array;
    };

    class ObjectValue : public Base
    {
    public:
      ObjectValue()
        : Base(objectValue)
      {
      }

      bool empty() const
      {
        return map.empty();
      }

      std::map<std::string, Value> map;
    };

  };

int main() {
  Json::Value ja(Json::arrayValue);

  ja.reserve(10);
  if (ja.empty())
    std::cout << "EMPTY!" << std::endl;

  return 0;
}