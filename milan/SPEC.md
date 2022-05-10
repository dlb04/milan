# Grammar

TranslationUnit := ExternalDecl
ExternalDecl := FunctionDecl
	| empty
FunctionDecl := "fn" /identifier/ Type "{}"
Type := int
	| void

```console
fn main() {
}
```