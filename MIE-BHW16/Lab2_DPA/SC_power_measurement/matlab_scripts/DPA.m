% (C) Ing. Jiri Bucek, Petr Vyleta

more off
tic
disp load

tracesLengthFile = fopen('traceLength.txt','r');
traceLength      = fscanf(tracesLengthFile, '%d');
numOfTraces      = 200;
startPoint       = 0;
points           = traceLength;
plaintextLength  = 16;

traces = tracesInput('traces.bin', traceLength, startPoint ,points, numOfTraces);
toc
disp('mean correction')
mm     = mean(mean(traces));
tm     = mean(traces, 2);
traces = traces - tm(:,ones(1,size(traces,2))) + mm;
toc

%kontrola zarovnani
%plot(traces(:,1:200)')

disp('load text')
inputs = plaintextInput('plaintext.txt', plaintextLength, numOfTraces);

disp('power hypotheses')
load tab.mat

disp('**** Add your code to complete the analysis here ****')
%plot(traces(:,100))

%graph = traces(:,5000:280000)';

for BYTE = 1:16
    
    powerHypothesis=zeros(numOfTraces,256);
    for TRACE = 0:200
        for K = 0:255
            powerHypothesis(numOfTraces, K) =  SubBytes(bitxor(inputs(TRACE, 1), K-1)+1));
        end
    end;
end;

CC = myCorrcoef(powerHypothesis, trace);



%for n_traces=1:200
%  for n_bytes=1:256
%    pHypothesis(n_traces,n_bytes)=SubBytes(bitxor(inputs(n_traces,1),n_bytes-1)+1);
%  end
%C = myCorrcoef(pHypothesis,traces); 
%plot(C)

%plot(graph);


%print out the key
key = 0:15;
for i=1:16
    fprintf( '0x%2.2X ',key(i));
end;

