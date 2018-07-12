

package main

import "fmt"
import "CodeGenTest/gotest"

func main() {
	fmt.Printf("Hello\n")
	t := gotest.Testobj {Oid: 4}
	fmt.Printf("%d", t.Oid)
}
