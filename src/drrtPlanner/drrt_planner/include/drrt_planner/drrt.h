/**************************************************************************
drrt.h
Header of the Drrt(Dynamic rrt) class

Hongbiao Zhu(hongbiaz@andrew.cmu.edu)
05/25/2020

**************************************************************************/

#ifndef DRRT_H_
#define DRRT_H_

#include "kdtree/kdtree.h"
#include "drrt_planner/drrt_base.h"
#include "drrt_planner/dual_state_graph.h"
#include "drrt_planner/dual_state_frontier.h"
#include "octomap_world/octomap_manager.h"

using namespace Eigen;
namespace drrt_planner_ns
{
class Drrt
{
public:
  Drrt(volumetric_mapping::OctomapManager* manager, DualStateGraph* graph, DualStateFrontier* frontier);
  ~Drrt();

  typedef Vector3d StateVec;
  bool plannerReady_;
  bool global_plan_;
  bool global_plan_pre_;
  bool local_plan_;
  bool nextNodeFound_;
  bool remainingFrontier_;
  int global_vertex_size_;
  int NextBestNodeIdx_;  // this is for global planner that still can find global frontier
  int bestNodeId_;       // this is for global plan that cannot find global frontier

  pcl::PointXYZ selectedGlobalFrontier_;
  StateVec root_;

  void clear();
  void setParams(Params params);
  void setRootWithOdom(const nav_msgs::Odometry& pose);
  void setTerrainCLoud();
  void setTerrainVoxelElev();
  void getThreeLocalFrontierPoint();
  void getNextNodeToClosestGlobalFrontier();
  void plannerInit();
  void plannerIterate();
  void pruneTree(StateVec root);
  void publishNode();
  bool gainFound();
  bool remainingLocalFrontier();
  bool generateRrtNodeToLocalFrontier(StateVec& newNode);
  bool collisionCheckByTerrain(StateVec origin_point, StateVec goal_point);
  bool inSensorRange(StateVec& node);
  bool inPlanningBoundary(StateVec node);
  bool inGlobalBoundary(StateVec node);
  int getNodeCounter();
  double angleDiff(StateVec direction1, StateVec direction2);
  double getZvalue(double x_position, double z_position);
  double gain(StateVec state);

protected:
  kdtree* kdTree_;
  Params params_;
  bool localPlanOnceMore_;
  int iterationCount_;
  int nodeCounter_;
  int remainingNodeCount_;
  std::vector<int> executedBestNodeList_;
  double bestGain_;
  double minX_;
  double minY_;
  double minZ_;
  double maxX_;
  double maxY_;
  double maxZ_;
  Eigen::Vector3d frontier1_direction_, frontier2_direction_, frontier3_direction_;
  pcl::PointCloud<pcl::PointXYZ>::Ptr localThreeFrontier_ =
      pcl::PointCloud<pcl::PointXYZ>::Ptr(new pcl::PointCloud<pcl::PointXYZ>());
  pcl::PointCloud<pcl::PointXYZ>::Ptr globalThreeFrontier_ =
      pcl::PointCloud<pcl::PointXYZ>::Ptr(new pcl::PointCloud<pcl::PointXYZ>());
  pcl::PointCloud<pcl::PointXYZI>::Ptr terrain_point_ =
      pcl::PointCloud<pcl::PointXYZI>::Ptr(new pcl::PointCloud<pcl::PointXYZI>());
  pcl::PointCloud<pcl::PointXYZI>::Ptr terrain_point_crop_ =
      pcl::PointCloud<pcl::PointXYZI>::Ptr(new pcl::PointCloud<pcl::PointXYZI>());

  Node* bestNode_;
  Node* rootNode_;
  std::vector<Node*> node_array;
  std::vector<double> terrain_voxle_elev_;
  volumetric_mapping::OctomapManager* manager_;
  DualStateGraph* dual_state_graph_;
  DualStateFrontier* dual_state_frontier_;
};
}

#endif  // DRRT_H
