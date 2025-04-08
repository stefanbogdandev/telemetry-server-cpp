#!/bin/bash

SERVER_URL="http://localhost:8080"

echo "========================================"
echo "==== Normal Event Insertions (user_signup) ===="
echo "========================================"

echo "Posting first event to /paths/user_signup..."
curl -s -X POST "$SERVER_URL/paths/user_signup" -H "Content-Type: application/json" -d '{"values":[5,4,3,6,7,8,9,1,2,3],"date":1712300000}'
echo -e "\n"

echo "Posting second event to /paths/user_signup..."
curl -s -X POST "$SERVER_URL/paths/user_signup" -H "Content-Type: application/json" -d '{"values":[15,14,13,16,17,18,19,11,12,13],"date":1712394000}'
echo -e "\n"

echo "Posting third event to /paths/user_signup..."
curl -s -X POST "$SERVER_URL/paths/user_signup" -H "Content-Type: application/json" -d '{"values":[25,24,23,26,27,28,29,21,22,23],"date":1712480000}'
echo -e "\n"

echo "----------------------------------------"
echo "Query: Mean (user_signup) - no timestamp filter"
curl -s -X GET "$SERVER_URL/paths/user_signup/meanLength" -H "Content-Type: application/json" -d '{"resultUnit":"seconds"}'
echo -e "\n"

echo "----------------------------------------"
echo "Test: Mean (user_signup) with startTimestamp and endTimestamp (should include second event only)"
curl -s -X GET "$SERVER_URL/paths/user_signup/meanLength" -H "Content-Type: application/json" -d '{
    "resultUnit": "seconds",
    "startTimestamp": 1712390000,
    "endTimestamp": 1712398000
}'
echo -e "\n"

echo "----------------------------------------"
echo "Test: Mean (user_signup) with startTimestamp and endTimestamp (no matching events)"
curl -s -X GET "$SERVER_URL/paths/user_signup/meanLength" -H "Content-Type: application/json" -d '{
    "resultUnit": "seconds",
    "startTimestamp": 1712000000,
    "endTimestamp": 1712100000
}'
echo -e "\n"

echo "----------------------------------------"
echo "Test: Mean (user_signup) with only startTimestamp"
curl -s -X GET "$SERVER_URL/paths/user_signup/meanLength" -H "Content-Type: application/json" -d '{
    "resultUnit": "seconds",
    "startTimestamp": 1712390000
}'
echo -e "\n"

echo "----------------------------------------"
echo "Test: Mean (user_signup) with only endTimestamp"
curl -s -X GET "$SERVER_URL/paths/user_signup/meanLength" -H "Content-Type: application/json" -d '{
    "resultUnit": "seconds",
    "endTimestamp": 1712398000
}'
echo -e "\n"

echo "========================================"
echo "==== Testing different event paths ===="
echo "========================================"

echo "Posting event to /paths/checkout..."
curl -s -X POST "$SERVER_URL/paths/checkout" -H "Content-Type: application/json" -d '{"values":[100,200,300,400,500,600,700,800,900,1000],"date":1712305000}'
echo -e "\n"

echo "Posting event to /paths/profile_update..."
curl -s -X POST "$SERVER_URL/paths/profile_update" -H "Content-Type: application/json" -d '{"values":[10,20,30,40,50,60,70,80,90,100],"date":1712310000}'
echo -e "\n"

echo "----------------------------------------"
echo "Query: Mean for /checkout"
curl -s -X GET "$SERVER_URL/paths/checkout/meanLength" -H "Content-Type: application/json" -d '{"resultUnit":"seconds"}'
echo -e "\n"

echo "Query: Mean for /profile_update"
curl -s -X GET "$SERVER_URL/paths/profile_update/meanLength" -H "Content-Type: application/json" -d '{"resultUnit":"milliseconds"}'
echo -e "\n"

echo "========================================"
echo "==== Error Handling Tests ===="
echo "========================================"

echo "Test: Invalid JSON (missing brace)..."
curl -s -X POST "$SERVER_URL/paths/error_test" -H "Content-Type: application/json" -d '{"values":[1,2,3,4,5,6,7,8,9,10],"date":1712394000'
echo -e "\n"

echo "Test: Missing required field (no values)..."
curl -s -X POST "$SERVER_URL/paths/error_test" -H "Content-Type: application/json" -d '{"date":1712394000}'
echo -e "\n"

echo "Test: Wrong type for values (string instead of array)..."
curl -s -X POST "$SERVER_URL/paths/error_test" -H "Content-Type: application/json" -d '{"values":"should_be_array","date":1712394000}'
echo -e "\n"

echo "----------------------------------------"
echo "Test: Correct valid POST after errors (error_test)"
curl -s -X POST "$SERVER_URL/paths/error_test" -H "Content-Type: application/json" -d '{"values":[1,2,3,4,5,6,7,8,9,10],"date":1712394000}'
echo -e "\n"

echo "========================================"
echo "==== All tests completed successfully ===="
echo "========================================"
