ALTER TABLE `objects`
  DROP `basicFlags`,
  DROP `size`,
  DROP `weight`;

ALTER TABLE `object_instances`
  DROP `basicFlags`,
  DROP `size`,
  DROP `weight`;
