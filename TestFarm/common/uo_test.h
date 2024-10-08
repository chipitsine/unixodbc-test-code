#ifndef UO_TEST_H
#define UO_TEST_H

#define UO_TEST_SUCCESS    0 /* Flying colors */
#define UO_TEST_FAILURE    1 /* General Failure */
#define UO_TEST_NA         2 /* Test not applicable to driver/datasource */
#define UO_TEST_WARNING    3 /* Something's fishy, or could not determine test outcome */
#define UO_TEST_INCIDENTAL 4 /* Main body of test passes, but something broken noticed on the way */
#define UO_TEST_INTERNAL   5 /* Something went wrong with the test itself */

#endif
