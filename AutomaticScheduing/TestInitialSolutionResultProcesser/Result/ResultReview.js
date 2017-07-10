jQuery(document).ready(function(){
    jQuery.getJSON("ProcessResultTestInitialSolution.json", function(data){
        showValue(data);
        showPercentage(data);
        showAccumulateValue(data);
        showAccumulatePercentage(data);
    });
});

function showValue(data) {
    jQuery('#ValueViewContainer').highcharts({
        chart: {
            type: 'area'
        },
        title: {
            text: '初始解值分布（10k次）'
        },
        xAxis: {
            categories: data.xAxis,
            tickmarkPlacement: 'on',
            title: {
                enabled: false
            }
        },
        tooltip: {
            split: true,
            valueSuffix: '次'
        },
        plotOptions: {
            area: {
                stacking: 'normal',
                lineColor: '#666666',
                lineWidth: 1,
                marker: {
                    lineWidth: 1,
                    lineColor: '#666666'
                }
            }
        },
        series: [{
            name: '初始解值分布',
            data: data.Value
        }]
    });
};

function showPercentage(data) {
    jQuery('#PercentageViewContainer').highcharts({
        chart: {
            type: 'area'
        },
        title: {
            text: '初始解值分布百分比（10k次）'
        },
        xAxis: {
            categories: data.xAxis,
            tickmarkPlacement: 'on',
            title: {
                enabled: false
            }
        },
        tooltip: {
            split: true,
            valueSuffix: '%'
        },
        plotOptions: {
            area: {
                stacking: 'normal',
                lineColor: '#666666',
                lineWidth: 1,
                marker: {
                    lineWidth: 1,
                    lineColor: '#666666'
                }
            }
        },
        series: [{
            name: '初始解值分布百分比',
            data: data.Percentage
        }]
    });
}

function showAccumulateValue(data) {
    jQuery('#AccumulateValueViewContainer').highcharts({
        chart: {
            type: 'area'
        },
        title: {
            text: '初始解值分布累积值（10k次）'
        },
        xAxis: {
            categories: data.xAxis,
            tickmarkPlacement: 'on',
            title: {
                enabled: false
            }
        },
        tooltip: {
            split: true,
            valueSuffix: '%'
        },
        plotOptions: {
            area: {
                stacking: 'normal',
                lineColor: '#666666',
                lineWidth: 1,
                marker: {
                    lineWidth: 1,
                    lineColor: '#666666'
                }
            }
        },
        series: [{
            name: '初始解值分布累积值',
            data: data.Accumulate_Value
        }]
    });
};

function showAccumulatePercentage(data) {
    jQuery('#AccumulatePercentageViewContainer').highcharts({
        chart: {
            type: 'area'
        },
        title: {
            text: '初始解值分布累积百分比（10k次）'
        },
        xAxis: {
            categories: data.xAxis,
            tickmarkPlacement: 'on',
            title: {
                enabled: false
            }
        },
        tooltip: {
            split: true,
            valueSuffix: '%'
        },
        plotOptions: {
            area: {
                stacking: 'normal',
                lineColor: '#666666',
                lineWidth: 1,
                marker: {
                    lineWidth: 1,
                    lineColor: '#666666'
                }
            }
        },
        series: [{
            name: '初始解值分布累积百分比',
            data: data.Accumulate_Percentage
        }]
    });
}