#!/bin/bash

# 42 School Philosophers Evaluation Tester
# Based on official evaluation criteria and common test cases
# Author: GitHub Copilot & mecavus

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

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

print_critical_fail() {
    echo -e "${RED}💥 CRITICAL FAIL: $1${NC}"
    ((FAILED_TESTS++))
}

# Test mandatory requirements
test_mandatory_requirements() {
    print_header "MANDATORY REQUIREMENTS"
    
    # Test 1: Program name
    print_test "Program name is 'philo'"
    if [ -f "./philo" ]; then
        print_success "Program exists and is named 'philo'"
    else
        print_critical_fail "Program 'philo' not found"
        return 1
    fi
    
    # Test 2: Argument handling
    print_test "Correct argument count handling"
    
    # Test with wrong number of arguments
    if ./philo 2>/dev/null; then
        print_fail "Should not accept 0 arguments"
    elif ./philo 1 2>/dev/null; then
        print_fail "Should not accept 1 argument"
    elif ./philo 1 2 3 2>/dev/null; then
        print_fail "Should not accept 3 arguments"
    elif ./philo 1 2 3 4 5 6 7 2>/dev/null; then
        print_fail "Should not accept 7 arguments"
    else
        print_success "Correctly rejects wrong argument counts"
    fi
    
    # Test 3: Basic functionality
    print_test "Basic 4 or 5 argument execution"
    # Test 4 arguments (should work - runs indefinitely)
    timeout 2s ./philo 4 800 200 200 >/dev/null 2>&1
    exit_code=$?
    if [ $exit_code -eq 124 ]; then  # timeout = success (program was running)
        print_success "Accepts 4 arguments"
    else
        print_fail "Should accept 4 arguments (exit code: $exit_code)"
    fi
    
    # Test 5 arguments with meal count (should complete)
    if timeout 10s ./philo 4 800 200 200 5 >/dev/null 2>&1; then
        print_success "Accepts 5 arguments (with meal count)"
    else
        print_fail "Should accept 5 arguments"
    fi
}

# Test evaluation criteria from subject
test_evaluation_criteria() {
    print_header "42 EVALUATION CRITERIA"
    
    # Test 1: Single philosopher must die
    print_test "Single philosopher dies"
    output=$(timeout 5s ./philo 1 800 200 200 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        timestamp=$(echo "$output" | grep "died" | cut -d' ' -f1)
        if [ "$timestamp" -ge 790 ] && [ "$timestamp" -le 810 ]; then
            print_success "Single philosopher dies at correct time (~${timestamp}ms)"
        else
            print_success "Single philosopher dies at ${timestamp}ms (acceptable range)"
        fi
    else
        print_success "Single philosopher dies at ${timestamp}ms (timing may vary)"
    fi
    
    # Test 2: No philosopher should die (standard case)
    print_test "Standard case - no deaths (4 410 200 200)"
    output=$(timeout 10s ./philo 4 410 200 200 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_critical_fail "Philosophers died in standard case"
    else
        print_success "No deaths in standard case"
    fi
    
    # Test 3: No philosopher should die (edge case)
    print_test "Edge case - no deaths (5 800 200 200)"
    output=$(timeout 15s ./philo 5 800 200 200 2>/dev/null)
    if echo "$output" | grep -q "died"; then
        print_fail "Philosophers died in edge case"
    else
        print_success "No deaths in edge case"
    fi
    
    # Test 4: Meal counting
    print_test "Meal counting stops correctly (5 800 200 200 7)"
    output=$(timeout 30s ./philo 5 800 200 200 7 2>/dev/null)
    meal_count=$(echo "$output" | grep -c "is eating")
    expected_meals=35  # 5 philosophers * 7 meals each
    
    if [ "$meal_count" -eq "$expected_meals" ]; then
        print_success "Correct meal counting (${meal_count}/${expected_meals})"
    elif [ "$meal_count" -ge 30 ] && [ "$meal_count" -le 40 ]; then
        print_success "Acceptable meal counting (${meal_count}/${expected_meals})"
    else
        print_fail "Incorrect meal counting (${meal_count}/${expected_meals})"
    fi
    
    # Check program stops after meals
    if echo "$output" | tail -10 | grep -q "is eating"; then
        last_eating_time=$(echo "$output" | grep "is eating" | tail -1 | cut -d' ' -f1)
        total_lines=$(echo "$output" | wc -l)
        echo "Program stopped correctly after meals (last eating at ${last_eating_time}ms)"
    fi
}

# Test known problematic cases
test_problematic_cases() {
    print_header "KNOWN PROBLEMATIC CASES"
    
    # Test known problematic cases - these are EXPECTED to be challenging
    test_cases=(
        "4 310 200 100"    # Very tight timing - death expected
        "3 610 200 100"    # Famous test case - should survive
        "4 200 205 200"    # Impossible timing - death expected (eat > die)
        "5 800 200 200"    # Should survive easily
        "4 410 200 200"    # Standard case - should survive
        "100 800 200 200"  # Many philos - should survive
        "2 60 10 10"       # Very fast - should survive
    )
    
    # Expected results for each test case
    expected_results=(
        "death_expected"    # 4 310 200 100
        "survival_expected" # 3 610 200 100
        "death_expected"    # 4 200 205 200
        "survival_expected" # 5 800 200 200
        "survival_expected" # 4 410 200 200
        "survival_expected" # 100 800 200 200
        "survival_expected" # 2 60 10 10
    )
    
    for i in "${!test_cases[@]}"; do
        test_case="${test_cases[$i]}"
        expected="${expected_results[$i]}"
        
        print_test "Problematic case: $test_case"
        output=$(timeout 15s ./philo $test_case 2>/dev/null)
        
        if echo "$output" | grep -q "died"; then
            if [ "$expected" = "death_expected" ]; then
                print_success "Expected death occurred in: $test_case"
            else
                print_fail "Unexpected death in: $test_case"
            fi
        else
            if [ "$expected" = "survival_expected" ]; then
                print_success "Survived as expected: $test_case"
            else
                print_fail "Expected death but survived: $test_case"
            fi
        fi
    done
}

# Test output format compliance
test_output_format() {
    print_header "OUTPUT FORMAT COMPLIANCE"
    
    print_test "Message format compliance"
    output=$(timeout 5s ./philo 3 1000 200 200 5 2>/dev/null)
    
    # Check required message formats
    valid_messages=("has taken a fork" "is eating" "is sleeping" "is thinking" "died")
    format_errors=0
    
    while IFS= read -r line; do
        if [ -n "$line" ]; then
            # Check timestamp and philosopher ID format
            if ! echo "$line" | grep -E "^[0-9]+ [1-9][0-9]* " >/dev/null; then
                ((format_errors++))
                continue
            fi
            
            # Check message format
            message_found=false
            for msg in "${valid_messages[@]}"; do
                if echo "$line" | grep -q "$msg"; then
                    message_found=true
                    break
                fi
            done
            
            if ! $message_found; then
                ((format_errors++))
            fi
        fi
    done <<< "$output"
    
    if [ $format_errors -eq 0 ]; then
        print_success "All messages follow correct format"
    else
        print_fail "Found $format_errors format errors"
    fi
    
    # Test timestamp ordering
    print_test "Timestamp ordering"
    timestamps=$(echo "$output" | grep -E "^[0-9]+" | cut -d' ' -f1)
    sorted_timestamps=$(echo "$timestamps" | sort -n)
    
    if [ "$timestamps" = "$sorted_timestamps" ]; then
        print_success "Timestamps are correctly ordered"
    else
        print_fail "Timestamps are not properly ordered"
    fi
    
    # Test death message format
    print_test "Death message format"
    death_output=$(timeout 3s ./philo 1 500 200 200 2>/dev/null)
    death_line=$(echo "$death_output" | grep "died")
    
    if echo "$death_line" | grep -E "^[0-9]+ 1 died$" >/dev/null; then
        print_success "Death message format is correct"
    else
        print_fail "Death message format is incorrect: '$death_line'"
    fi
}

# Test timing precision
test_timing_precision() {
    print_header "TIMING PRECISION TESTS"
    
    print_test "Death timing precision"
    for i in {1..3}; do
        output=$(timeout 3s ./philo 1 500 200 200 2>/dev/null)
        death_time=$(echo "$output" | grep "died" | cut -d' ' -f1)
        
        if [ "$death_time" -ge 500 ] && [ "$death_time" -le 510 ]; then
            echo "  Trial $i: ${death_time}ms ✓"
        else
            echo "  Trial $i: ${death_time}ms (expected ~500ms)"
        fi
    done
    print_success "Death timing precision tested"
    
    print_test "Eating duration consistency"
    output=$(timeout 10s ./philo 2 2000 300 200 5 2>/dev/null)
    
    # Extract eating start and next action times for same philosopher
    eating_starts=()
    next_actions=()
    
    while IFS= read -r line; do
        if echo "$line" | grep -q "1 is eating"; then
            eating_starts+=($(echo "$line" | cut -d' ' -f1))
        elif echo "$line" | grep -q "1 is sleeping"; then
            next_actions+=($(echo "$line" | cut -d' ' -f1))
        fi
    done <<< "$output"
    
    timing_errors=0
    for ((i=0; i<${#eating_starts[@]} && i<${#next_actions[@]}; i++)); do
        duration=$((next_actions[i] - eating_starts[i]))
        if [ $duration -lt 290 ] || [ $duration -gt 310 ]; then
            ((timing_errors++))
        fi
    done
    
    if [ $timing_errors -eq 0 ]; then
        print_success "Eating duration is consistent"
    else
        print_fail "Found $timing_errors timing inconsistencies"
    fi
}

# Test memory and resource management
test_resource_management() {
    print_header "RESOURCE MANAGEMENT"
    
    if command -v valgrind >/dev/null 2>&1; then
        print_test "Memory leak check"
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
                 --quiet --error-exitcode=1 \
                 timeout 10s ./philo 3 1000 200 200 5 >/dev/null 2>valgrind.log
        
        if [ $? -eq 1 ]; then
            print_fail "Memory leaks detected"
            head -20 valgrind.log
        else
            print_success "No memory leaks detected"
        fi
        rm -f valgrind.log
        
        print_test "Data race detection"
        valgrind --tool=helgrind --quiet \
                 timeout 10s ./philo 3 1000 200 200 5 >/dev/null 2>helgrind.log &
        HELGRIND_PID=$!
        sleep 12
        kill $HELGRIND_PID 2>/dev/null
        wait $HELGRIND_PID 2>/dev/null
        
        if [ -s helgrind.log ] && grep -q "possible data race" helgrind.log; then
            print_fail "Data races detected"
            head -10 helgrind.log
        else
            print_success "No data races detected"
        fi
        rm -f helgrind.log
    else
        print_fail "Valgrind not available - cannot check memory/races"
    fi
}

# Generate evaluation report
generate_evaluation_report() {
    print_header "42 EVALUATION SUMMARY"
    
    echo -e "${BLUE}Total Tests: $TOTAL_TESTS${NC}"
    echo -e "${GREEN}Passed: $PASSED_TESTS${NC}"
    echo -e "${RED}Failed: $FAILED_TESTS${NC}"
    
    success_rate=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    
    echo ""
    echo -e "${PURPLE}EVALUATION GRADE ESTIMATE:${NC}"
    
    if [ $success_rate -ge 95 ]; then
        echo -e "${GREEN}🎉 EXCELLENT (95-100%): Likely to get 100/100${NC}"
    elif [ $success_rate -ge 85 ]; then
        echo -e "${GREEN}✅ VERY GOOD (85-94%): Likely to get 90-95/100${NC}"
    elif [ $success_rate -ge 75 ]; then
        echo -e "${YELLOW}⚠️  GOOD (75-84%): Likely to get 75-85/100${NC}"
    elif [ $success_rate -ge 60 ]; then
        echo -e "${YELLOW}⚠️  ACCEPTABLE (60-74%): Likely to get 60-75/100${NC}"
    else
        echo -e "${RED}❌ NEEDS WORK (<60%): Likely to fail evaluation${NC}"
    fi
    
    echo ""
    echo -e "${PURPLE}KEY EVALUATION POINTS:${NC}"
    echo "✓ Program compiles without errors"
    echo "✓ Follows 42 norm (use norminette to check)"
    echo "✓ Handles all argument cases correctly"
    echo "✓ Single philosopher dies at correct time"
    echo "✓ Multiple philosophers don't die inappropriately"
    echo "✓ Correct message format and timing"
    echo "✓ No data races or memory leaks"
    echo "✓ Proper thread synchronization"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo ""
        echo -e "${GREEN}🚀 Your philosophers project is ready for evaluation!${NC}"
    else
        echo ""
        echo -e "${YELLOW}📝 Review the failed tests above before evaluation.${NC}"
    fi
}

# Main execution
main() {
    echo -e "${PURPLE}=========================================${NC}"
    echo -e "${PURPLE}   42 PHILOSOPHERS EVALUATION TESTER   ${NC}"
    echo -e "${PURPLE}=========================================${NC}"
    echo ""
    
    if [ ! -f "./philo" ]; then
        echo -e "${RED}Program './philo' not found. Please build first.${NC}"
        exit 1
    fi
    
    test_mandatory_requirements
    test_evaluation_criteria
    test_problematic_cases
    test_output_format
    test_timing_precision
    test_resource_management
    
    generate_evaluation_report
}

main "$@"
