sudo sysctl -w kernel.perf_event_paranoid=0
sudo perf record ./gravity_sim


Samples: 336K of event 'cycles', Event count (approx.): 318124563938
Samples: 800K of event 'cycles', Event count (approx.): 840604052734
Samples: 666K of event 'cycles', Event count (approx.): 679733924870
Samples: 691K of event 'cycles', Event count (approx.): 591300722528


fix so it plots contours knowing phi is proportional to 1/r