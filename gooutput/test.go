

package main

import "fmt"
import "CodeGenTest/test"

func main() {
	fmt.Printf("Hello\n")
	t := test.Testobj {Oid: 4}
	fmt.Printf("%d", t.Oid)
}
