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