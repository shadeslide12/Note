# CFD Solver Lua Scripts

This directory contains example Lua scripts for automating CFD computations.

## Available Scripts

### 1. simple_run.lua
A basic script that demonstrates:
- Starting a computation with default parameters
- Monitoring progress
- Waiting for completion
- Saving results

**Usage**: Load this script in the Script tab and click Run.

### 2. temperature_sweep.lua
An advanced script that performs parameter sweep analysis:
- Runs multiple computations with different inlet temperatures
- Temperature range: 280K to 320K (step: 10K)
- Automatically saves results for each run
- Demonstrates loop control and state monitoring

**Usage**: Load this script and click Run. It will automatically run 5 computations.

### 3. test_api.lua
A comprehensive API test script:
- Tests all available Lua API functions
- Demonstrates parameter setting
- Shows state monitoring
- Quick 30-iteration test run

**Usage**: Use this to verify the Lua integration is working correctly.

## Lua API Reference

### Solver Control Functions

#### solver.start(iterations, [inlet_pressure], [inlet_temperature], [ambient_temperature])
Start a new computation.
- `iterations` (required): Number of iterations to run
- `inlet_pressure` (optional): Inlet pressure in Pa (default: 101325)
- `inlet_temperature` (optional): Inlet temperature in K (default: 300)
- `ambient_temperature` (optional): Ambient temperature in K (default: 293)

**Example**:
```lua
solver.start(100)  -- Use all defaults
solver.start(100, 101325, 310, 293)  -- Custom parameters
```

#### solver.stop()
Stop the currently running computation.

**Example**:
```lua
solver.stop()
```

#### solver.continue()
Continue a stopped computation.

**Example**:
```lua
solver.continue()
```

### Query Functions

#### solver.getState()
Get the current computation state.

**Returns**: String - "Not_Started", "Running", "Stopped", or "Completed"

**Example**:
```lua
local state = solver.getState()
print("Current state: " .. state)
```

#### solver.getCurrentIteration()
Get the current iteration number.

**Returns**: Integer - Current iteration (0 if not started)

**Example**:
```lua
local iter = solver.getCurrentIteration()
print("Iteration: " .. iter)
```

### File Operations

#### solver.saveResults(filename)
Save computation results to a CSV file.

**Parameters**:
- `filename`: Output filename (string)

**Example**:
```lua
solver.saveResults("results.csv")
solver.saveResults("results_temp_300K.csv")
```

### Utility Functions

#### print(...)
Print messages to the output console.

**Parameters**: Variable number of arguments (will be converted to strings)

**Example**:
```lua
print("Hello, World!")
print("Temperature:", 300, "K")
```

#### sleep(milliseconds)
Pause script execution for the specified duration.

**Parameters**:
- `milliseconds`: Sleep duration in milliseconds (integer)

**Example**:
```lua
sleep(1000)  -- Sleep for 1 second
sleep(500)   -- Sleep for 0.5 seconds
```

## Writing Your Own Scripts

### Basic Template

```lua
-- Your script description
print("=== My CFD Script ===")

-- Start computation
solver.start(100, 101325, 300, 293)

-- Wait for completion
while solver.getState() ~= "Completed" do
    sleep(1000)
end

-- Save results
solver.saveResults("my_results.csv")
print("Done!")
```

### Parameter Sweep Template

```lua
-- Parameter sweep
for param = start_value, end_value, step do
    print("Running with parameter: " .. param)
    
    solver.start(iterations, param, ...)
    
    while solver.getState() ~= "Completed" do
        sleep(500)
    end
    
    solver.saveResults("results_" .. param .. ".csv")
end
```

### Progress Monitoring Template

```lua
solver.start(1000)

while solver.getState() ~= "Completed" do
    local iter = solver.getCurrentIteration()
    if iter % 100 == 0 then
        print("Progress: " .. iter .. " iterations")
    end
    sleep(1000)
end
```

## Tips

1. **Always check state**: Use `solver.getState()` to verify computation status
2. **Use sleep()**: Add delays to avoid busy-waiting
3. **Save results**: Use unique filenames for each run in parameter sweeps
4. **Error handling**: Check script output console for errors
5. **Stop button**: Use the Stop button to interrupt long-running scripts

## Troubleshooting

### Script doesn't start
- Check for syntax errors in the output console
- Verify all required parameters are provided

### Computation doesn't complete
- Check if computation encountered an error
- Verify iteration count is reasonable
- Check output console for error messages

### Results not saved
- Verify filename is valid
- Check file permissions
- Ensure computation completed successfully

## Advanced Usage

### Conditional Execution

```lua
local state = solver.getState()
if state == "Not_Started" then
    solver.start(100)
elseif state == "Stopped" then
    solver.continue()
end
```

### Multiple Runs with Different Parameters

```lua
local configs = {
    {iter=50, temp=280},
    {iter=100, temp=300},
    {iter=150, temp=320}
}

for i, config in ipairs(configs) do
    print("Run " .. i .. ": " .. config.iter .. " iterations, " .. config.temp .. "K")
    solver.start(config.iter, 101325, config.temp, 293)
    
    while solver.getState() ~= "Completed" do
        sleep(500)
    end
    
    solver.saveResults("results_run" .. i .. ".csv")
end
```

## Standard Lua Libraries Available

The following Lua standard libraries are available:
- `base`: Basic functions (print, type, tonumber, etc.)
- `math`: Mathematical functions (math.sin, math.cos, math.sqrt, etc.)
- `string`: String manipulation (string.format, string.sub, etc.)
- `table`: Table operations (table.insert, table.remove, etc.)

Refer to the [Lua 5.3 Reference Manual](https://www.lua.org/manual/5.3/) for details.
