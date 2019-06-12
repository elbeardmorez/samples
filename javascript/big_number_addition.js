// add numbers as strings
var add_ = (s1, s2) => {
    let res = "";
    let l_s1 = s1.length;
    let l_s2 = s2.length;
    if (l_s2 > l_s1) {
        let s1_ = s2;
        s2 = s1;
        s1 = s1_;
        let l_s1_ = l_s2;
        l_s2 = l_s1;
        l_s1 = l_s1_;
    }
    let carry = 0;
    let idx = 0;
    let i1 = 0;
    let i2 = 0;
    let sum = "";
    //console.log(`adding s1: ${s1}, s2: ${s2}, l_s1: ${l_s1}, l_s2: ${l_s2}`);
    while (idx < l_s1) {
        i1 = parseInt(s1.substr(l_s1 - idx - 1, 1));
        i2 = 0;
        if (idx < l_s2)
            i2 = parseInt(s2.substr(l_s2 - idx - 1, 1));
        //console.log(`adding i1: ${i1}, i2: ${i2}`);
        sum = (i1 + i2 + carry).toString();
        if (sum.length === 1) {
            carry = 0;
            res = sum + res;
        } else {
            res = sum.substr(sum.length - 1) + res;
            carry = parseInt(sum.substr(0, sum.length -1));
        }
        idx++;
    }
    if (carry !== 0)
        res = carry.toString() + res;
    return res;
}
