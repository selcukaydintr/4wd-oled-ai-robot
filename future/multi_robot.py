class RobotNetwork:
    def __init__(self):
        self.robots = {}
        self.shared_map = SharedMap()
        self.task_allocator = TaskAllocator()
        
    def add_robot(self, robot_id, capabilities):
        self.robots[robot_id] = {
            'status': 'active',
            'position': None,
            'tasks': [],
            'capabilities': capabilities
        }
        
    def coordinate_tasks(self):
        # Görev dağıtımı ve koordinasyon
        available_tasks = self.get_pending_tasks()
        robot_assignments = self.task_allocator.assign_tasks(
            available_tasks,
            self.robots
        )
        self.broadcast_assignments(robot_assignments)
        
    def share_map_updates(self, robot_id, map_data):
        # Harita güncelleme ve senkronizasyon
        self.shared_map.merge_update(robot_id, map_data)
        self.broadcast_map_update(robot_id, map_data) 