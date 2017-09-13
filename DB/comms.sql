//noc_join
//creates a table which joins ID values with simulation data
CREATE TABLE noc_join AS SELECT NOC2.id, NOC.src_instance, NOC.src_port, NOC.sync_instance, NOC.sync_port, NOC2.time, NOC2.pkt_id, NOC2.header, NOC2.vc, NOC2.src, NOC2.dest FROM NOC INNER JOIN NOC2 ON NOC.id = NOC2.id;

SELECT * FROM noc_join;

//Freq Counter
//Creates a table which sorts by frequency of appearance
SELECT id, src_instance, src_port, sync_instance, sync_port, COUNT(*) AS frequency from noc_join group by id, noc_join.src_instance, noc_join.src_port, noc_join.sync_instance, noc_join.sync_port ORDER BY COUNT(*) DESC;

//Isolate by Channel Name
//Return only with given channel name
SELECT * FROM noc_join WHERE ID IN (SELECT ID from noc_join where src_instance = 'SLR_0_VNOC_1_NPS_0_L');

//Isolate by ID
//Given an ID, return table with only given ID
SELECT * FROM noc_join WHERE ID IN (SELECT ID from noc_join where id = '6');
