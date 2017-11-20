program exemplo75 (input, output);
label 100,200;
var m,n : integer;
    procedure p(var k:integer; s:integer);
    var x,z : integer;
    begin
        read (m,n,k,s,x,z);
    100: k := 6;
	     x := 5;
		 goto 200;
    end;
    function f(test:integer):integer;
    begin
        read (n,f);
        if n < 0 then 
        else 
            f:=f(f(n)+1);
        goto 100;
    end;
begin
     p(f(m),n);
	 n:= f(m);
	 write(f(f(n)+1));
end.
    