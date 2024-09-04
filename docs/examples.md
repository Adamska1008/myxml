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
element root = doc.root();
// or just query elem
optional<element> elem = doc.first_elem();
// or query by name
optional<element> elem = doc.first_elem("elem");
```

### Element

#### Attributes

```C++
// create an element with name root
element elem("root");
// or 
element elem = element::parse(xml);
// query attributes, it returns an optional string reference
fmt::println(elem["hello"]);
// modify attribute, will create new attribute if not found one
elem["hello"] = "world!";
// if key not exist, query it will create an empty attribute
elem["hello"]; // == elem["hello"] = "";
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
