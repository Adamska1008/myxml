# myxml

**myxml** is a xml dom parser which aims to be simple, stable and easy to use. 

## Features

- **Text Lazy-Loading**
- **Rich Test Cases**
- **Easy-to-use Api**

## Why not tinyxml2

**tinyxml2** implements a useful xml parser. However, it aims to provide functionality in all conditions and abandons STL, RTTI or exceptions. Such design rule may cause several issues. It causes level of instability, for example [issue932](https://github.com/leethomason/tinyxml2/issues/932). It implements attributes with linked list, while hashmap may be better. With the power of STL, **myxml** can get element by name and get attribute in O(1) time complexity.

**tinyxml2** is designed to load all data into memory, with parent block being responsible for managing the memory of its child. **myxml** make full use of smart pointer, thus enabling chunked processing. This will relieve memory pressure when loading large files.

## Example



## Inspiration

This repo is inspired by [tinyxml2](https://github.com/leethomason/tinyxml2) and [serde](https://crates.io/crates/serde).