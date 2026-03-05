-- Simple CFD Computation Script
-- This script demonstrates basic computation control

print("=== Simple CFD Computation ===")
print("Starting computation with 100 iterations...")

-- Start computation with default parameters
-- Parameters: iterations, inlet_pressure (Pa), inlet_temperature (K), ambient_temperature (K)
solver.start(100, 101325, 300, 293)

print("Computation started!")
print("Current state: " .. solver.getState())

-- Wait for computation to complete
print("Waiting for computation...")
while solver.getState() ~= "Completed" do
    sleep(1000)  -- Sleep for 1 second
    local iteration = solver.getCurrentIteration()
    if iteration > 0 and iteration % 20 == 0 then
        print("Progress: " .. iteration .. " iterations completed")
    end
end

print("Computation completed!")
print("Final state: " .. solver.getState())
print("Total iterations: " .. solver.getCurrentIteration())

-- Save results
print("Saving results...")
solver.saveResults("results_simple.csv")
print("Done!")
