function send()
    return 0
end

function receive(value)
    local socket = require("socket")
    local server = socket.tcp()
    server:bind("127.0.0.1", 12345) # 1
    server:listen(1) # 2

    while true do
        local client = server:accept()
        local received = client:receive()
        local data = tostring(tonumber(received) + 1) .. "\n"
        client:send(data)
        client:close()
    end
end

function send_count()
    return 0
end