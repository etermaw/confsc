# confsc
Simple parser for configuration script. Script has following syntax:

```
name value
another_name [group, of, values]

named_object{
    named_param_1 a
    named_param_2 b
}

#comment (single line)
```

On c++ side, API is quite simple:
```
using dict = std::unordered_map<string_view, std::vector<string_view>>;
void parse(string_view file, dict& values, std::unordered_map<string_view, dict>& named_objects);
```

After parsing, values will contain 2 values:
"name" : ["value"]
"another_name" : ["group", "of", "values"]

and named_objects will contain:
"named_object" : [{"named_param_1" : "a"}, {"named_param_2" : "b"}]
