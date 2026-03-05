-- API Test Script
-- This script tests all available Lua API functions

print("=== CFD Solver Lua API Test ===")
print("")

-- Test 1: Print function
print("Test 1: Print function")
print("Hello from Lua!")
print("Multiple", "arguments", "test:", 123, 456.789)
print("")

-- Test 2: Get initial state
print("Test 2: Get initial state")
local state = solver.getState()
print("Current state: " .. state)
local iteration = solver.getCurrentIteration()
print("Current iteration: " .. iteration)
print("")

-- Test 3: Parameter setting (informational only)
print("Test 3: Parameter setting")
solver.setIterations(50)
solver.setInletPressure(101325)
solver.setInletTemperature(300)
solver.setAmbientTemperature(293)
print("")

-- Test 4: Start computation
print("Test 4: Start computation")
print("Starting with 30 iterations...")
solver.start(30, 101325, 300, 293)
print("Start command sent")
print("")

-- Test 5: Monitor progress
print("Test 5: Monitor progress")
sleep(2000)  -- Wait 2 seconds
state = solver.getState()
iteration = solver.getCurrentIteration()
print("State after 2s: " .. state)
print("Iteration after 2s: " .. iteration)
print("")

-- Test 6: Wait for completion
print("Test 6: Wait for completion")
while solver.getState() ~= "Completed" do
    sleep(500)
end
print("Computation completed!")
print("Final iteration: " .. solver.getCurrentIteration())
print("")

-- Test 7: Save results
print("Test 7: Save results")
solver.saveResults("test_results.csv")
print("")

print("=== All API Tests Completed ===")
