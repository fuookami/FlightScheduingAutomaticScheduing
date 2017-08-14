jQuery(document).ready(function() {
    jQuery.getJSON("ResultProcess.json", function(data) {
        showQuantities(data.Quantities);
        showScores(data.Scores);
        showBestContinuingIter(data.BestContinuingIterators);
        showMutationRate(data.MutationRate);
    });
});

function showQuantities(data) {
    jQuery("#QuantitiesContainer").highcharts({
        chart: {
            type: 'arearange',
            zoomType: 'x'
        },
        title: {
            text: '种群密度变化图'
        },
        credits: {
            enabled: false // 禁用版权信息
        },
        xAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '代数'
            }
        },
        yAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '种群密度（个）'
            }
        },
        tooltip: {
            split: true,
            valueSuffix: '个'
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
            name: '种群密度',
            data: data
        }]
    });
}

function showScores(data) {
    jQuery("#ScoresContainer").highcharts({
        chart: {
            type: 'arearange',
            zoomType: 'x'
        },
        title: {
            text: '波及延误变化图'
        },
        credits: {
            enabled: false // 禁用版权信息
        },
        xAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '代数'
            }
        },
        yAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '波及延误（分钟）'
            }
        },
        tooltip: {
            split: true,
            valueSuffix: '分钟'
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
            name: '波及延误',
            data: data
        }]
    });
}

function showBestContinuingIter(data) {
    jQuery("#BestIteratorsContainer").highcharts({
        chart: {
            type: 'spline'
        },
        title: {
            text: '最优解持续代数变化图'
        },
        credits: {
            enabled: false // 禁用版权信息
        },
        xAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '代数'
            }
        },
        yAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '最优解持续代数（代）'
            }
        },
        tooltip: {
            split: true,
            valueSuffix: '代'
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
            name: '最优解持续代数',
            data: data
        }]
    });
}

function showMutationRate(data) {
    jQuery("#MutationRateContainer").highcharts({
        chart: {
            type: 'spline'
        },
        title: {
            text: '基因变异概率变化图'
        },
        credits: {
            enabled: false // 禁用版权信息
        },
        xAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '代数'
            }
        },
        yAsis: {
            labels: {
                style: {
                    fontSize: '32px'
                }
            },
            title: {
                text: '基因变异概率（%）'
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
            name: '最优解持续代数',
            data: data
        }]
    });
}