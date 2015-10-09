# An implementation of the Debouncing algorithm
The Debouncer object samples button status and records most-recent samples in its array.
Only when all array elements are the same, the button status is considered as stable so the debounced value of the button can be used.
* otherwise button_debouced output a number that is neither `0` or `1`, indicating that the button is still bouncing.
