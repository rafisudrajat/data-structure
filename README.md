# data-structure
Algorithm and data structure fundamental implemented in C++

# How to write test case
Testing framework for used in this repository is Google Test, usually test case is written like this
```C++
    TEST(test_suite_name,test_name){
        //Test preparation, including mocking, etc

        //Call to the code that we test

        //Assertion. Check if test result is correct or not
    }
```
1. To test a method:
    a. test_suite_name should be like this: `Class_MethodTest`
    b. test_name should be like this `ShouldExpectedValueWhenConditionToTest`
2. To test a function:
    a. test_suite_name should be like this: `FunctionNameTest`
    b. test_name should be like this `ShouldExpectedValueWhenConditionToTest`

For parametric test

