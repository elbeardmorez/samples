package main

import (
	"os"
	"fmt"
	"strconv"
	"strings"
)

func numeric_range(args ...int32) []int32 {

	var n1, n2, x int32
	switch (len(args)) {
		case 1: n1 = 0; n2 = args[0]; x = 1
		case 2: n1 = args[0]; n2 = args[1]; x = 1
		case 3: n1 = args[0]; n2 = args[1]; x = args[2]
	}
	res := make([]int32, 0)
	for l := n1; l < n2; l += x {
		res = append(res, l);
	}
	return res
}

func main() {
	if ! (len(os.Args) > 1 && len(os.Args) < 5) {
		panic("[error] invalid number of arguments")
	}
	args := make([]int32, 0)
	for _, v := range(os.Args[1:]) {
		i, ok := strconv.ParseInt(v, 10, 32)
		if (ok != nil) {
			panic(ok)
		}
		args = append(args, int32(i))
	}
	var vals []string
	for _, i := range(numeric_range(args...)) {
		vals = append(vals, strconv.Itoa((int(i))))
	}
	fmt.Printf("%v\n", strings.Join(vals, " "))
}
