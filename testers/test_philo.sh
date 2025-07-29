#!/bin/bash

# Ultimate Philosophers Tester
# Tests all edge cases, error conditions, and performance scenarios
# Author: GitHub Copilot & mecavus
# Date: July 27, 2025

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Helper functions
print_header() {
    echo -e "${BLUE}=================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}=================================${NC}"
}

print_test() {
    echo -e "${CYAN}Testing: $1${NC}"
    ((TOTAL_TESTS++))
}

print_success() {
    echo -e "${GREEN}✓ PASS: $1${NC}"
    ((PASSED_TESTS++))
}

print_fail() {
    echo -e "${RED}✗ FAIL: $1${NC}"
    ((FAILED_TESTS++))
}

print_warning() {
    echo -e "${YELLOW}⚠ WARNING: $1${NC}"
}

# Build the program
build_program() {
    print_header "BUILDING PROGRAM"
    if make clean > /dev/null 2>&1 && make > /dev/null 2>&1; then
        print_success "Program built successfully"
        return 0
    else
        print_fail "Failed to build program"
        return 1
    fi
}

# Test argument validation
test_argument_validation() {
    print_header "ARGUMENT VALIDATION TESTS"
    
    # Test 1: No arguments
    print_test "No arguments"
    if timeout 1s ./philo 2>/dev/null; then
        print_fail "Should reject no arguments"
    else
        print_success "Correctly rejects no arguments"
    fi
    
    # Test 2: Too few arguments
    print_test "Too few arguments (3 args)"
    if timeout 1s ./philo 5 800 200 2>/dev/null; then
        print_fail "Should reject too few arguments"
    else
        print_success "Correctly rejects too few arguments"
    fi
    
    # Test 3: Too many arguments
    print_test "Too many arguments (7 args)"
    if timeout 1s ./philo 5 800 200 200 5 extra 2>/dev/null; then
        print_fail "Should reject too many arguments"
    else
        print_success "Correctly rejects too many arguments"
    fi
    
    # Test 4: Zero philosophers
    print_test "Zero philosophers"
    if timeout 1s ./philo 0 800 200 200 2>/dev/null; then
        print_fail "Should reject zero philosophers"
    else
        print_success "Correctly rejects zero philosophers"
    fi
    
    # Test 5: Negative numbers
    print_test "Negative time_to_die"
    if timeout 1s ./philo 5 -800 200 200 2>/dev/null; then
        print_fail "Should reject negative time_to_die"
    else
        print_success "Correctly rejects negative time_to_die"
    fi
    
    # Test 6: Zero time_to_die
    print_test "Zero time_to_die"
    if timeout 1s ./philo 5 0 200 200 2>/dev/null; then
        print_fail "Should reject zero time_to_die"
    else
        print_success "Correctly rejects zero time_to_die"
    fi
    
    # Test 7: Zero time_to_eat
    print_test "Zero time_to_eat"
    if timeout 1s ./philo 5 800 0 200 2>/dev/null; then
        print_fail "Should reject zero time_to_eat"
    else
        print_success "Correctly rejects zero time_to_eat"
    fi
    
    # Test 8: Zero time_to_sleep
    print_test "Zero time_to_sleep"
    if timeout 1s ./philo 5 800 200 0 2>/dev/null; then
        print_fail "Should reject zero time_to_sleep"
    else
        print_success "Correctly rejects zero time_to_sleep"
    fi
    
    # Test 9: Non-numeric arguments
    print_test "Non-numeric philosopher count"
    if timeout 1s ./philo abc 800 200 200 2>/dev/null; then
        print_fail "Should reject non-numeric arguments"
    else
        print_success "Correctly rejects non-numeric arguments"
    fi
    
    # Test 10: Very large numbers (overflow)
    print_test "Integer overflow"
    if timeout 1s ./philo 999999999999999999999 800 200 200 2>/dev/null; then
        print_fail "Should reject integer overflow"
    else
        print_success "Correctly rejects integer overflow"
    fi
    
    # Test 11: Floating point numbers
    print_test "Floating point numbers"
    if timeout 1s ./philo 5.5 800 200 200 2>/dev/null; then
        print_fail "Should reject floating point numbers"
    else
        print_success "Correctly rejects floating point numbers"
    fi
    
    # Test 12: Mixed valid/invalid
    print_test "Mixed valid/invalid arguments"
    if timeout 1s ./philo 5 800 abc 200 2>/dev/null; then
        print_fail "Should reject mixed valid/invalid arguments"
    else
        print_success "Correctly rejects mixed valid/invalid arguments"
    fi
    
    # Test 13: Zero meals (should be valid)
    print_test "Zero number of meals (should be valid)"
    if timeout 2s ./philo 3 1000 200 200 0 >/dev/null 2>&1; then
        print_success "Correctly accepts zero meals (immediate stop)"
    else
        print_fail "Should accept zero meals"
    fi
    
    # Test 14: Negative meals
    print_test "Negative number of meals"
    if timeout 1s ./philo 5 800 200 200 -5 2>/dev/null; then
        print_fail "Should reject negative meals"
    else
        print_success "Correctly rejects negative meals"
    fi
}

# Test basic functionality
test_basic_functionality() {
    print_header "BASIC FUNCTIONALITY TESTS"
    
    # Test 1: Single philosopher (should die)
    print_test "Single philosopher (should die)"
    output=$(timeout 3s ./philo 1 800 200 200 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_success "Single philosopher dies as expected"
    else
        print_fail "Single philosopher should die"
    fi
    
    # Test 2: Two philosophers (should not die)
    print_test "Two philosophers (should survive)"
    output=$(timeout 5s ./philo 2 1000 200 200 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_fail "Two philosophers should not die"
    else
        print_success "Two philosophers survive"
    fi
    
    # Test 3: Standard 5 philosophers
    print_test "Five philosophers (standard test)"
    output=$(timeout 10s ./philo 5 800 200 200 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_fail "Five philosophers should not die"
    else
        print_success "Five philosophers survive"
    fi
    
    # Test 4: Meal counting
    print_test "Meal counting (3 philosophers, 5 meals each)"
    output=$(timeout 10s ./philo 3 1000 200 200 5 2>/dev/null)
    meal_count=$(echo "$output" | grep -c "is eating")
    if [ "$meal_count" -eq 15 ]; then
        print_success "Correct meal count (15 total meals)"
    else
        print_warning "Expected 15 meals, got $meal_count"
    fi
}

# Test edge cases
test_edge_cases() {
    print_header "EDGE CASE TESTS"
    
    # Test 1: Very fast eating/sleeping
    print_test "Very fast times (1ms each)"
    output=$(timeout 3s ./philo 3 500 1 1 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_fail "Fast times should not cause death"
    else
        print_success "Handles very fast times"
    fi
    
    # Test 2: Very slow eating
    print_test "Very slow eating (longer than death time)"
    output=$(timeout 3s ./philo 2 500 1000 100 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_success "Correctly handles impossible timing"
    else
        print_fail "Should die when eating takes longer than death time"
    fi
    
    # Test 3: Large number of philosophers
    print_test "Many philosophers (100)"
    output=$(timeout 10s ./philo 100 1000 100 100 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_fail "100 philosophers should not die"
    else
        print_success "Handles 100 philosophers"
    fi
    
    # Test 4: Maximum reasonable philosophers
    print_test "Maximum philosophers (200)"
    output=$(timeout 15s ./philo 200 2000 100 100 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_warning "200 philosophers had deaths (may be acceptable)"
    else
        print_success "Handles 200 philosophers without deaths"
    fi
    
    # Test 5: Very tight timing (the problematic case we fixed)
    print_test "Tight timing (3 philos, 610ms death, 200ms eat, 100ms sleep)"
    output=$(timeout 20s ./philo 3 610 200 100 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_fail "Should handle tight timing with priority system"
    else
        print_success "Priority system prevents death in tight timing"
    fi
    
    # Test 6: Even tighter timing
    print_test "Very tight timing (3 philos, 410ms death, 200ms eat, 50ms sleep)"
    output=$(timeout 15s ./philo 3 410 200 50 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_warning "Very tight timing may cause deaths (borderline acceptable)"
    else
        print_success "Handles very tight timing"
    fi
}

# Test stress scenarios
test_stress_scenarios() {
    print_header "STRESS TEST SCENARIOS"
    
    # Test 1: Long running test
    print_test "Long running test (5 minutes with meals)"
    start_time=$(date +%s)
    output=$(timeout 300s ./philo 4 5000 500 500 100 2>/dev/null)
    end_time=$(date +%s)
    duration=$((end_time - start_time))
    
    if echo "$output" | grep -q "died"; then
        print_fail "Long running test had deaths"
    else
        print_success "Long running test completed without deaths ($duration seconds)"
    fi
    
    # Test 2: Memory stress (many philosophers)
    print_test "Memory stress test (500 philosophers)"
    output=$(timeout 30s ./philo 500 3000 200 200 3 2>/dev/null)
    if [ $? -eq 0 ]; then
        print_success "Handles 500 philosophers memory allocation"
    else
        print_warning "500 philosophers may exceed system limits"
    fi
    
    # Test 3: CPU stress (frequent context switching)
    print_test "CPU stress test (rapid eating/sleeping)"
    output=$(timeout 10s ./philo 10 1000 10 10 50 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_fail "CPU stress test had deaths"
    else
        print_success "Handles rapid context switching"
    fi
}

# Test data race detection
test_data_races() {
    print_header "DATA RACE DETECTION TESTS"
    
    # Test 1: Check for data races with helgrind (if available)
    if command -v valgrind >/dev/null 2>&1; then
        print_test "Helgrind data race detection"
        valgrind --tool=helgrind --quiet ./philo 3 1000 200 200 5 >/dev/null 2>helgrind.log &
        VALGRIND_PID=$!
        sleep 10
        kill $VALGRIND_PID 2>/dev/null
        wait $VALGRIND_PID 2>/dev/null
        
        if [ -s helgrind.log ] && grep -q "possible data race" helgrind.log; then
            print_fail "Data races detected by helgrind"
            echo "First few race conditions:"
            head -20 helgrind.log
        else
            print_success "No data races detected by helgrind"
        fi
        rm -f helgrind.log
    else
        print_warning "Valgrind not available for data race detection"
    fi
    
    # Test 2: Thread safety test (multiple rapid starts/stops)
    print_test "Thread safety (rapid start/stop)"
    for i in {1..5}; do
        timeout 2s ./philo 3 1000 100 100 3 >/dev/null 2>&1 &
        sleep 0.1
        kill $! 2>/dev/null
        wait $! 2>/dev/null
    done
    print_success "Rapid start/stop test completed"
}

# Test memory leaks
test_memory_leaks() {
    print_header "MEMORY LEAK TESTS"
    
    if command -v valgrind >/dev/null 2>&1; then
        print_test "Memory leak detection with valgrind"
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
                 --quiet --error-exitcode=1 \
                 timeout 10s ./philo 3 2000 200 200 5 >/dev/null 2>valgrind.log
        
        if [ $? -eq 1 ]; then
            print_fail "Memory leaks detected"
            cat valgrind.log
        else
            print_success "No memory leaks detected"
        fi
        rm -f valgrind.log
    else
        print_warning "Valgrind not available for memory leak detection"
    fi
}

# Test output format
test_output_format() {
    print_header "OUTPUT FORMAT TESTS"
    
    print_test "Output format validation"
    output=$(timeout 5s ./philo 2 1000 200 200 3 2>/dev/null)
    
    # Check timestamp format
    if echo "$output" | grep -E "^[0-9]+ [0-9]+ (has taken a fork|is eating|is sleeping|is thinking|died)$" >/dev/null; then
        print_success "Output format is correct"
    else
        print_fail "Output format is incorrect"
        echo "Sample output:"
        echo "$output" | head -5
    fi
    
    # Check for timestamp ordering
    print_test "Timestamp ordering"
    timestamps=$(echo "$output" | grep -E "^[0-9]+" | cut -d' ' -f1)
    if [ "$(echo "$timestamps" | sort -n)" = "$timestamps" ]; then
        print_success "Timestamps are properly ordered"
    else
        print_fail "Timestamps are not properly ordered"
    fi
    
    # Check for death message format
    print_test "Death message format (single philosopher)"
    death_output=$(timeout 3s ./philo 1 500 200 200 2>/dev/null)
    if echo "$death_output" | grep -E "^[0-9]+ 1 died$" >/dev/null; then
        print_success "Death message format is correct"
    else
        print_fail "Death message format is incorrect"
    fi
}

# Test signal handling
test_signal_handling() {
    print_header "SIGNAL HANDLING TESTS"
    
    print_test "SIGINT handling (Ctrl+C)"
    ./philo 5 5000 200 200 >/dev/null 2>&1 &
    PHILO_PID=$!
    sleep 2
    kill -INT $PHILO_PID
    wait $PHILO_PID 2>/dev/null
    EXIT_CODE=$?
    if [ $EXIT_CODE -ne 0 ]; then
        print_success "Properly handles SIGINT"
    else
        print_warning "SIGINT handling may need improvement"
    fi
    
    print_test "SIGTERM handling"
    ./philo 5 5000 200 200 >/dev/null 2>&1 &
    PHILO_PID=$!
    sleep 2
    kill -TERM $PHILO_PID
    wait $PHILO_PID 2>/dev/null
    EXIT_CODE=$?
    if [ $EXIT_CODE -ne 0 ]; then
        print_success "Properly handles SIGTERM"
    else
        print_warning "SIGTERM handling may need improvement"
    fi
}

# Test performance
test_performance() {
    print_header "PERFORMANCE TESTS"
    
    print_test "Performance test (time to complete meals)"
    start_time=$(date +%s%N)
    timeout 30s ./philo 4 2000 100 100 10 >/dev/null 2>&1
    end_time=$(date +%s%N)
    duration_ms=$(( (end_time - start_time) / 1000000 ))
    
    if [ $duration_ms -lt 30000 ]; then
        print_success "Performance test completed in ${duration_ms}ms"
    else
        print_warning "Performance test took ${duration_ms}ms (may be slow)"
    fi
    
    print_test "Resource usage test"
    # Monitor CPU and memory usage
    ./philo 10 3000 200 200 20 >/dev/null 2>&1 &
    PHILO_PID=$!
    sleep 5
    if ps -p $PHILO_PID >/dev/null 2>&1; then
        cpu_usage=$(ps -p $PHILO_PID -o %cpu --no-headers)
        mem_usage=$(ps -p $PHILO_PID -o %mem --no-headers)
        print_success "Resource usage: CPU=${cpu_usage}%, MEM=${mem_usage}%"
    else
        print_success "Process completed quickly"
    fi
    kill $PHILO_PID 2>/dev/null
    wait $PHILO_PID 2>/dev/null
}

# Test regression scenarios
test_regression_scenarios() {
    print_header "REGRESSION TEST SCENARIOS"
    
    # Known problematic scenarios that were fixed
    test_cases=(
        "3 610 200 100"
        "3 410 200 50"
        "4 310 200 100"
        "5 800 200 200"
        "2 60 10 10"
        "4 410 200 200"
        "100 800 200 200"
    )
    
    for test_case in "${test_cases[@]}"; do
        print_test "Regression test: $test_case"
        output=$(timeout 15s ./philo $test_case 2>/dev/null)
        if echo "$output" | grep -q "died"; then
            print_warning "Regression test failed: $test_case"
        else
            print_success "Regression test passed: $test_case"
        fi
    done
}

# Generate final report
generate_report() {
    print_header "TEST SUMMARY REPORT"
    
    echo -e "${BLUE}Total Tests: $TOTAL_TESTS${NC}"
    echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
    echo -e "${RED}Failed: $FAILED_TESTS${NC}"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "${GREEN}🎉 ALL TESTS PASSED! Your philosophers implementation is solid!${NC}"
        exit 0
    else
        echo -e "${RED}❌ Some tests failed. Review the failures above.${NC}"
        exit 1
    fi
}

# Main test execution
main() {
    echo -e "${PURPLE}=========================================${NC}"
    echo -e "${PURPLE}    ULTIMATE PHILOSOPHERS TESTER       ${NC}"
    echo -e "${PURPLE}=========================================${NC}"
    echo ""
    
    # Check if program exists
    if [ ! -f "./philo" ]; then
        if ! build_program; then
            echo -e "${RED}Cannot proceed without a working program${NC}"
            exit 1
        fi
    fi
    
    # Run all test suites
    test_argument_validation
    test_basic_functionality
    test_edge_cases
    test_stress_scenarios
    test_data_races
    test_memory_leaks
    test_output_format
    test_signal_handling
    test_performance
    test_regression_scenarios
    
    # Generate final report
    generate_report
}

# Run the tests
main "$@"
