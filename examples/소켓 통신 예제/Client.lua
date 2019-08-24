function send()
    local result = received
    received = nil
    return result
end

function receive(value)
    local socket = require("socket")
    local client = socket.tcp()
    client:connect("127.0.0.1", 12345) # 1
    client:send(tostring(value) .. "\n")
    local temp = client:receive()
    local temp_len = string.len(temp)
    received = tonumber(string.sub(temp, 1, temp_len))
    client:close()
end

function send_count()
    return received == nil and 0 or 1
end