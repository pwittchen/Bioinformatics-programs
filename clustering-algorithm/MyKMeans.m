% Introduction to Bioinformatics - laboratory no. 3 - Clustering algorithms
% Author: Piotr Wittchen <piotr.wittchen@gmail.com> 
% Macrofaculty - AEI - sem. 6 - spec. Informatics
% The Silesian University of Technology
% Academic year: 2009/2010

% myKMeans function
% points - given points (0 for random values)
% means  - calculated clusters (0 for random values)
% assign - assigned points
% N - number of data points
% K - number of clusters
% D - number of dimensions
% I - max. iterations
function MyKMeans(points, means, assign, N, K, D, I)

    disp('Clustering algorithm');
    disp('Please wait, calculations in progress...');

    % input parameters

    %numberOfPoints      = 1000;
    %numberOfClusters    = 4;
    %numberOfIterations  = 1000;
    drawPlot            = 1;
    
    numberOfPoints      = N;
    numberOfClusters    = K;
    numberOfIterations  = I;
    %numberOfDimensions  = D;

    a = 0;   % lower bound of the random numbers distribution
    b = 100; % upper bound of the random numbers distribution

    % generating random points

    if(points == 0)
        pointsX   = a + (b - a) .* rand(1,numberOfPoints);
        pointsY   = a + (b - a) .* rand(1,numberOfPoints);  
    else
        pointsX   = points(1,1:numberOfPoints);
        pointsY   = points(2,1:numberOfPoints);
    end
    
    if(means == 0)
        clustersX = a + (b - a) .* rand(1,numberOfClusters);
        clustersY = a + (b - a) .* rand(1,numberOfClusters);
    else
        clustersX   = means(1,1:numberOfPoints);
        clustersY   = means(2,1:numberOfPoints);
    end

    % 1st stop condition 
    % performing an algorithm given number of iterations
    for iterations = 1:numberOfIterations

        % calculating distances between clusters and points

        distances = zeros(numberOfClusters, numberOfPoints);

        for i = 1:numberOfClusters
            distances(i,1:1:numberOfPoints) = (sqrt(((pointsX - pointsY) + (clustersX(i) - clustersY(i))).^2));
        end

        % assigning points to clusters
        
        if(assign == 0)
            assignedPointsX  = zeros(numberOfClusters, numberOfPoints);
            assignedPointsY  = zeros(numberOfClusters, numberOfPoints);
        else
            assignedPointsX = assign(1,1:numberOfPoints);
            assignedPointsY = assign(2,1:numberOfPoints);
        end

        for j = 1:numberOfPoints
            [value, clusterNumber] = min(distances(1:1:numberOfClusters,j));
            assignedPointsX(clusterNumber, j) = pointsX(1,j);
            assignedPointsY(clusterNumber, j) = pointsY(1,j);
        end

        % calculating new cluster points

        for k = 1:numberOfClusters
            x = assignedPointsX(k,1:1:numberOfPoints);
            x = (x(find(x)))'; % removing zeros from the array and inverting the array
            y = assignedPointsY(k,1:1:numberOfPoints);
            y = (y(find(y)))';
            
            % 2nd stop condition - clusters don't change their positions
            if(clustersX(1,k) == mean(x) && clustersY(1,k) == mean(y))
                break;
            else
                clustersX(1,k) = mean(x); % calculating mean basing on the proper non-zero points
                clustersY(1,k) = mean(y);
            end
        end

    end

    disp('Calculations finished. Please, check data inside the workspace.');

    % plotting the data
    % commands for drawing the plots should be defined manually 
    % due to proper color settings

    if(drawPlot)
        disp('You can see the results on the plot.');
        clusterPlane = figure;
        hold on;
        plot(assignedPointsX(1,1:numberOfPoints),assignedPointsY(1,1:numberOfPoints),'r.');
        plot(assignedPointsX(2,1:numberOfPoints),assignedPointsY(2,1:numberOfPoints),'g.');
        plot(assignedPointsX(3,1:numberOfPoints),assignedPointsY(3,1:numberOfPoints),'b.');
        plot(assignedPointsX(4,1:numberOfPoints),assignedPointsY(4,1:numberOfPoints),'c.');
        plot(clustersX, clustersY,'kX');
        hold off;
    end
end