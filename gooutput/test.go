

package main

import "fmt"
import "CodeGenTest/test"

func main() {
	fmt.Printf("Hello\n")
	t := test.Testobj {Oid: 4}
  list, idtostring := t.Func1("test", 3, 3.0)
  _ = list
  _ = idtostring
}
