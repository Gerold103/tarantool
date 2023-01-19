## feature/replication

* New option `box.cfg.cluster_name` allows to assign a human-readable name to
  the entire cluster with all its replicasets. It has to match in all instances
  of all the replicasets in one cluster and is displayed in
  `box.info.cluster.name` (gh-5029).

* New option `box.cfg.replicaset_name` allows to assign a human-readable name to
  the replicaset. Works the same as `box.cfg.replicaset_uuid`. It has to match
  in all instances of one replicaset and is displayed in
  `box.info.replicaset.name` (gh-5029).
