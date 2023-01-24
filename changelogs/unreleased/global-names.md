## feature/replication

* New option `box.cfg.cluster_name` allows to assign a human-readable name to
  the entire cluster with all its replicasets. It has to match in all instances
  of all the replicasets in one cluster and is displayed in
  `box.info.cluster.name` (gh-5029).

* New option `box.cfg.replicaset_name` allows to assign a human-readable name to
  the replicaset. Works the same as `box.cfg.replicaset_uuid`. It has to match
  in all instances of one replicaset and is displayed in
  `box.info.replicaset.name` (gh-5029).

* New option `box.cfg.instance_name` allows to assign a human-readable name to
  one instance. Works the same as `box.cfg.instance_uuid`. The name has to be
  unique in its replicaset. It is displayed in `box.info.name`. Names of the
  other replicas in the same replicaset are visible in
  `box.info.replication[id].name` (gh-5029).
