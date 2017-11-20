program type2 (input, output);
type banana = integer;
var b : banana;
   x  : integer;
begin
   x := 1;
   for b:=1 to 10 do
      begin
         if (x <= b) 
            then
            x:=x+1;
         write (x)
      end
end.
