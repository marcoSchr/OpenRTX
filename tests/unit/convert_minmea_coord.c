#include <stdio.h>
#include <stdlib.h>
#include <gps.h>

void assert_conversion(struct minmea_float *f, int32_t expected)
{
    int32_t result;
    convert_minmea_coord_to_int(f, &result);
    if (result != expected)
    {
        printf("FAILED! result value %d - expected %d\n",
               result, expected);
        exit(1);
    }
}

int main() {
    printf("minmea coordinate conversion test\n");
    struct minmea_float test = {5333735, 1000};
    assert_conversion(&test, 53562250);
    test.scale = 1;
    test.value = 0;
    assert_conversion(&test, 0);
    test.scale = 1000;
    test.value = -5333735;
    assert_conversion(&test, -53562250);
    printf("PASS\n");

    return 0;
}