{ for { let z := 0 } 1 { } { z := 8 let x := 3 } let t := 2 }
// ====
// stackOptimization: true
// ----
//     /* "":17:18   */
//   0x00
//     /* "":6:20   */
//   pop
//     /* "":27:48   */
// tag_1:
//     /* "":34:35   */
//   0x08
//     /* "":36:46   */
//   pop
//     /* "":45:46   */
//   0x03
//     /* "":27:48   */
//   pop
//     /* "":23:26   */
//   jump(tag_1)
