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
            text: '初始解值分布（100k次）'
        },
        credits: {
            enabled:false // 禁用版权信息
        },
        xAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '波及延误（分钟）'
            }
        },
        yAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '初始解值频数（次）'
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
            data: data.Values
        }]
    });
};

function showPercentage(data) {
    jQuery('#PercentageViewContainer').highcharts({
        chart: {
            type: 'area'
        },
        title: {
            text: '初始解值分布（100k次）'
        },
        credits: {
            enabled:false // 禁用版权信息
        },
        xAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '波及延误（分钟）'
            }
        },
        yAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '初始解值频率（%）'
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
            text: '初始解值分布累积值（100k次）'
        },
        credits: {
            enabled:false // 禁用版权信息
        },
        xAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '波及延误（分钟）'
            }
        },
        yAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: "初始解值累积频数（次）"
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
            name: '初始解值分布累积值',
            data: data.AccumulateValue
        }]
    });
};

function showAccumulatePercentage(data) {
    jQuery('#AccumulatePercentageViewContainer').highcharts({
        chart: {
            type: 'area'
        },
        title: {
            text: '初始解值分布累积百分比（100k次）'
        },
        credits: {
            enabled:false // 禁用版权信息
        },
        xAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '波及延误（分钟）'
            }
        },
        yAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: "初始解值累积频率（%）"
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
            data: data.AccumulatePercentage
        }]
    });
}