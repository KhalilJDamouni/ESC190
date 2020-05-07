from lab0_utilities import *
	
class Languages:
	def __init__(self): #Given
		self.data_by_year = {}

	def build_trees_from_file(self, file_object): #Make
		# implement
		content = file_object.read()
		content = content.replace('\n', ',')
		content = content.split(',')

		all_nodes = []
		for n in range(3, len(content) - 2, 3):
			if(content[n+2].isdigit() and content[n+2].isdigit()):
				current_stat = LanguageStat(content[n + 1], int(content[n]), int(content[n + 2]))
				current_node = Node(current_stat)
				all_nodes.append(current_node)			

		#Insert Nodes Into dictionary
		current_year = all_nodes[0].val.year
		current_tree = BalancingTree(all_nodes[0])
		for x in range(1, len(all_nodes)): #Cycles through all nodes
			if(current_year != all_nodes[x].val.year): #If the years change, create a new tree
				self.data_by_year[current_year] = current_tree #Places the finished tree in the dictionary
				#print(current_tree.print_tree())
				current_tree = BalancingTree(all_nodes[x])
				current_year = all_nodes[x].val.year
			else: #Places the node into the tree of its year
				current_tree.balanced_insert(all_nodes[x])

		self.data_by_year[current_year] = current_tree
		#print(current_tree.print_tree())
		return self.data_by_year

	def query_by_name(self, language_name): #Make
		# implement
		output = {}
		for i in self.data_by_year:
			current_tree = self.data_by_year[i]
			output[i] = current_tree.find_value(language_name, current_tree.root)
		return output
		
	def query_by_count(self, threshold = 0): #Make
		# implement
		output = {}
		for i in self.data_by_year:
			current_tree = self.data_by_year[i]
			output[i] = (current_tree.traverse_preorder(current_tree.root, threshold, []))
		return output


class BalancingTree:
	def __init__(self, root_node): #Given
		self.root = root_node
	
	def balanced_insert(self, node, curr = None): #Given
		curr = curr if curr else self.root
		self.insert(node, curr)
		self.balance_tree(node)

	def insert(self, node, curr = None): #Given
		curr = curr if curr else self.root
		# insert at correct location in BST
		if node._val < curr._val:
			if curr.left is not None:
				self.insert(node, curr.left)
			else:
				node.parent = curr
				curr.left = node
		else:
			if curr.right is not None:
				self.insert(node, curr.right)
			else:
				node.parent = curr
				curr.right = node
		return

	def balance_tree(self, node): #Make
		#First make sure that there is an unbalanced node
		if(self.is_balanced()):
			return 
		
		#Travelling up, find the first unbalanced node
		while(node.parent != None):
			if(abs(self.get_bf(node.parent)) > 1):
				#print(node.parent.val.name,"got fucked")
				node = node.parent
				break
			else:
				node = node.parent
		#print(node.val.name, "is node")

		#Case a) (Z.bf = -2, y.bf < 0)
		if(self.get_bf(node) < -1):
			if(node.left and self.get_bf(node.left) < 0): #Right Rotate
				self.right_rotate(node)
				#print("Right Rotate")
			elif(node.left and self.get_bf(node.left) > 0): #Left Right
				self.left_rotate(node.left)
				self.right_rotate(node)
				#print("Left Right Rotate")
		elif(self.get_bf(node) > 1):
			if(node.right and self.get_bf(node.right) > 0):
				self.left_rotate(node)
				#print("Left")
			elif(node.right and self.get_bf(node.right) < 0):
				self.right_rotate(node.right)
				self.left_rotate(node)
				#print("Right Left")
			
		#Case b) (Z.bf = -2, y.bf > 0)

		#Case c) (Z.bf = 2, y.bf > 0)

		#Case d) (Z.bf = 2, y.bf < 0)
		pass
 
	def update_height(self, node): #Given
		node.height = 1 + max(self.height(node.left), self.height(node.right))

	def height(self, node): #Given
		return node.height if node else -1

	def left_rotate(self, z): #Given
		y = z.right
		y.parent = z.parent
		if y.parent is None:
			self.root = y
		else:
			if y.parent.left is z:
				y.parent.left = y
			elif y.parent.right is z:
				y.parent.right = y
		z.right = y.left
		if z.right is not None:
			z.right.parent = z
		y.left = z
		z.parent = y
		self.update_height(z)
		self.update_height(y)

	def right_rotate(self, z): #Make
		# implement
		# use left_rotate as a reference
		y = z.left
		y.parent = z.parent
		if y.parent is None:
			self.root = y
		else:
			if y.parent.right is z:
				y.parent.right = y
			elif y.parent.left is z:
				y.parent.left = y
		z.left = y.right
		if z.left is not None:
			z.left.parent = z
		y.right = z
		z.parent = y
		self.update_height(z)
		self.update_height(y)
		pass

	def find_balance_factor(self, node): #Make
		return self.get_bf(node)

	def is_balanced(self): #Make
		all_em = self.get_all_bfs(self.root, [])
		for i  in all_em:
			if(abs(i) > 1):
				return False
		return True

	def get_all_bfs(self, node, all_em): #Helper Method
		#self.update_height(node)
		if(node.right):
			all_em = self.get_all_bfs(node.right, all_em)
		if(node.left):
			all_em = self.get_all_bfs(node.left, all_em)
		all_em.append(self.get_bf(node))
		node.bf = self.get_bf(node)
		return all_em
	
	def get_bf(self,node): #Helper Method
		if node:
			return self.get_height(node.right) - self.get_height(node.left)
		else:
			return -1
	
	def get_height(self, node): #Helper Method
		if node:
			return 1 + max(self.get_height(node.left), self.get_height(node.right))
		return -1

	def traverse_preorder(self, node, threshold, output): #Helper Method
		#print(node._val,"has a bf of", node.bf)
		if(node.val.count > threshold):
			#print(node.val.name,"Has more than", threshold)
			output.append(node.val.name)
		if(node.right):
			self.traverse_preorder(node.right, threshold, output)
			#return output
		if(node.left):
			self.traverse_preorder(node.left, threshold, output)
			#return output
		return output

	def find_value(self, name, node): #Helper Method
		#print("Current Node is", node.val.name)
		if(name > node.val.name):
			return self.find_value(name, node.right)
		elif(name < node.val.name):
			return self.find_value(name, node.left)
		elif(node.val.name == name):
			#print("The count is ", node.val.count)
			return node.val.count
		else:
			return node.val.count

	def print_tree(self): #EXTRA JUST FOR DEBUG
		queueboi = [self.root]
		output = ""
		while queueboi:
			popboi = queueboi.copy()
			queueboi.clear()
			for boi in popboi:
				output += "{} ".format(boi.val.name[0])# + str(self.get_height(boi)) + " " + str(self.get_bf(boi)))
				if boi.left:
					queueboi += [boi.left]
				if boi.right:
					queueboi += [boi.right]
			output += "\n"
		
		return output
