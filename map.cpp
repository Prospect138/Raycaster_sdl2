#include "map.h"

#define _ 0
#define X 1

int map[16][16] = 
{{X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, X, X, X, X, X, X, X, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, _, _, _, _, _, _, _, _, _, _, _, _, _, _, X},
 {X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X}};

#undef X
#undef _
