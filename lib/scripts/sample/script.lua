-- the first program in every language

lastTime = 0

while true do
    if hare.getTime() - lastTime > 1 then
        lastTime = hare.getTime()
        print("fps:", hare.getTimer():getFPS())
    end
end
