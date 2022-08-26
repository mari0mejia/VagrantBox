// Sun Sep 22 18:01:45 PDT 2019
#include<ostream>
#include<string>
class Sample_singleton {
  public:
    // This function retrieves the Sample_singleton instance. 
    // It is a static member function--it is not associated with 
    // an instance (an object) of the class. 
    // Think of it as a "Traditional, C-style" function that is in the
    // Sample_singleton namespace
    static Sample_singleton& instance();
    friend std::ostream& operator<<(std::ostream& os, const Sample_singleton& sh);

    //Go ahead and write any member functions you require for your class
    std::string get_it();

    //The copy constructor and assignment operator are compiler-generated
    //functions. Here the compiler is instructed not to generate these
    //functions.  This is useful for all classes that don't explicitly 
    //require these functions (not just singletons).
    Sample_singleton(const Sample_singleton&) = delete;
    Sample_singleton& operator=(const Sample_singleton&) = delete;

  private:

    //Any private data you require for your class
    std::string string_data;
    int count_data;

    //Make the default constructor private. Only the static
    //variable object (See Sample_singleton::instance() function) 
    //has access to this private function, allowing it to be constructed.
    Sample_singleton() 
      :string_data("Special"), 
       count_data(0)
    {}
};


Sample_singleton& Sample_singleton::instance()
{
  //Making this variable static gives it global scope
  //with visibility local to this function
  static Sample_singleton the_instance;
  return the_instance;
}

std::string Sample_singleton::get_it()
{
  std::string ret;
  ++count_data;
  for(int i=0; i<count_data; ++i)
    ret+=string_data + " ";
  return ret;
}

std::ostream& operator<<(std::ostream& os, const Sample_singleton& sh)
{
  os << sh.count_data << " X " << sh.string_data;
  return os;
}

#include<iostream>
int main()
{
  Sample_singleton& thingy=Sample_singleton::instance();
  std::cout << thingy.get_it() << std::endl;
  std::cout << thingy.get_it() << std::endl;
//  std::cout << thingy.get_it() << std::endl;
  //invoke friend function: operator<<()
  std::cout << thingy << std::endl;
}
