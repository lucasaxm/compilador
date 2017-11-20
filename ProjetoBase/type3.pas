program type2 (input, output);
type banana  = integer;
   laranja = integer;
var b : banana;
   l  :  laranja;
   x  : integer;
begin
   x := 1;
   for b:=1 to 10 do
      begin
         l:=b;     (* Deve acusar erro aqui *) 
         x:=x+1;
         write (x)
      end
end.
