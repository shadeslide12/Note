-- Temperature Sweep Script
-- This script runs multiple computations with different inlet temperatures

print("=== Temperature Sweep Analysis ===")

-- Define temperature range
local temp_start = 280  -- K
local temp_end = 320    -- K
local temp_step = 10    -- K
local iterations = 50

print(string.format("Temperature range: %d K to %d K (step: %d K)", 
                    temp_start, temp_end, temp_step))
print(string.format("Iterations per run: %d", iterations))
print("")

-- Loop through temperatures
local run_count = 0
for temp = temp_start, temp_end, temp_step do
    run_count = run_count + 1
    
    print(string.format("--- Run %d: Inlet Temperature = %d K ---", run_count, temp))
    
    -- Start computation with current temperature
    solver.start(iterations, 101325, temp, 293)
    
    -- Wait for completion
    while solver.getState() ~= "Completed" do
        sleep(500)  -- Sleep for 0.5 seconds
    end
    
    print(string.format("Run %d completed!", run_count))
    
    -- Save results with temperature in filename
    local filename = string.format("results_temp_%dK.csv", temp)
    solver.saveResults(filename)
    print(string.format("Results saved to: %s", filename))
    print("")
    
    -- Small delay between runs
    sleep(1000)
end

print("=== Temperature Sweep Completed ===")
print(string.format("Total runs: %d", run_count))
print("All results saved!")
