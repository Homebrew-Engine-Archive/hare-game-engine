function factorial(n)
    if n == 0 then return 1
    else return n * factorial(n - 1) end
end

function test(n)
	io.write(n,"! = ",factorial(n),"\n")
end

for n=0,16 do
	test(n)
end
