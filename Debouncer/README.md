# An implementation of a Debouncing algorithm
The Debouncer object samples button status and records most-recent samples in its array.
Only when all array elements are the same, the button status is considered as `stable` so the debounced value of the button can be used.
* otherwise button_debouced output a number that is neither `0` or `1`, indicating that the button is still bouncing.

The output of `button_debouced()` has four possibilities:

1. `0`

2. `1`

3. `UNSTABLE`

4. `UNCHANGED`, in this case the button is stable but the state is not changed since last stable read. User code should decide what to do in this case.


