# Examples

## Document

### Simple

```C++
using myxml;
// `std::string xml`
document doc = document::parse(xml);
// or
document doc = document::load(path);
// get root elem
optional<element> elem = doc.root().first_elem();
// or directly query elem in root
optional<element> elem = doc.first_elem();
// or query by name
optional<element> elem = doc.first_elem("elem");
```

### Element

#### Attributes

```C++
// create an element with name 'root'
element elem("root");
// or get an element from parsing
element elem = element::parse(xml);
// or do it by custom string literal
element elem = "<root/>"_elem;
// query attributes, it returns an string reference
fmt::println(elem["hello"]);
// modify attribute, will create new attribute if not found one
elem["hello"] = "world!";
// if key not exist, query it will create an empty attribute
elem["hello"]; // == elem["hello"] = "";
// use attribute to get an optional reference
if (auto value = elem.attribute("hello"); value)
{
    fmt::println("element has attribute \"hello\" with value {}", *value);
}
```

#### Children

```C++
// element root;
element child = root.first_elem();
// or first element with name "child"
element child = root.first_elem("child");
// elements == vector<element>
std::vector<element> children = root.elems();
// or elements with same name
std::vector<element> children = root.elems("child");
```

### Text

```C++
// Simple Query
text txt = root.first_text();
// print raw
fmt::println(txt);
// print trimmed text, it returns std::string and will not modify it
fmt::println(txt.trimmed()); 
// yet it will modify. return a new text
fmt::println(txt.trim());
```