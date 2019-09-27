package main

import (
	"fmt"
	"os"
)

func reverse_string(s string) string {
	s_ := ""
	a_s := s[:]
	for l := len(a_s) - 1; l > -1; l-- {
		s_ += string(a_s[l])
	}
	return s_
}

func main() {
	if len(os.Args) < 2 {
		panic(fmt.Sprintf("[error] missing string arg"))
	}
	var s string = reverse_string(os.Args[1])
	fmt.Printf("%v\n", s)
}
