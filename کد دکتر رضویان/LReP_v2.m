  function result=LReP(i,ac,pk,l1)
  
      PiNew = pk(i)+l1*(1-pk(i));
      pn=[pk(1:(i-1)) pk((i+1):end)];
      PjNew=pn.*(1-l1) ;
      PkNew=[PjNew(1:(i-1)) PiNew PjNew(i:end)] ;
  
%   l1 = l1/eval ; % reward according to steps to goal
%   PiNew=pk(i)-pk(i)*(l1*beta)-pk(i)*(l2*(1-beta))+l1*beta ;
%   pn=[pk(1:(i-1)) pk((i+1):end)];
%   PjNew=pn-pn.*(l1*beta)+(pn.*(-1)+1/(ac-1)).*(l2*(1-beta)) ;
%   PkNew=[PjNew(1:(i-1)) PiNew PjNew(i:end)] ;
%          normal = 0 ; % normalization and exploration band
%          for count=1:size(PkNew,2)
%                if PkNew(count) < 0.00001
%                   PkNew(count) = 0.00001 ;
%                end
%                normal = normal + PkNew(count) ;
%          end
%          for count=1:size(PkNew,2)
%                PkNew(count) = PkNew(count)/normal ;
%          end
  result = PkNew;
  end