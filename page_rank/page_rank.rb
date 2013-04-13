#!/usr/bin/env ruby

##
# simple script to compute the page rank of web pages

class Ranker
  attr_accessor :pages, :rank

  def initialize(*pages)
    # puts all pages in args in an array
    @pages = pages
    # create a hash to store ranks with a default value of 1
    @rank  = Hash[pages.collect { |page| [page, 1] }]
  end

  def rank_pages(d)
    # store the previous page rank before new calculations
    save_rank

    @pages.each do |page|
      # for each page divide it's previous PR by the number of it's links
      # and sum all of that
      sum = page.linked_by.map { |l| (@previous_rank[l] / l.links.size.to_f) }.reduce(&:+)
      # if there was no link, give a 0 value for the sum
      sum ||= 0
      # do the math and store the result in the hash
      @rank[page] = (1-d) + (d * sum)
    end
  end

  def converging?
    # method to know when to stop:
    # difference of the sum of the previous page rank minus the current
    # is less than 0.01
    (@previous_rank.values.reduce(:+) - @rank.values.reduce(:+)) < 0.01
  end

  def show_ranks
    # just show it in the terminal
    @rank.each do |page,rank|
      puts "#{page.name}: #{rank}"
    end
  end

  private

    def save_rank
      # copy current values to be stored as previous
      @previous_rank = @rank.clone
    end
end

class Page
  attr_accessor :name, :links, :linked_by

  def initialize(name)
    @name         = name
    @links        = []
    @linked_by    = []
  end

  def add_link(*pages)
    # adding a link means there are two things to do:
    pages.each do |page|
      # adding the linked page
      @links << page
      # and tell the linked page it is linked by current page
      page.linked_by << self
    end
  end
end

# just some examples
a = Page.new('A')
b = Page.new('B')
c = Page.new('C')
d = Page.new('D')

# we add some links to our pages
# we can add as many as we want, no need to put them in an array
a.add_link(b,c)
b.add_link(c,d)
c.add_link(b)

# now create our ranker
# and give him the pages we want to rank
ranker = Ranker.new(a,b,c,d)

# let's start at iteration 1
iteration = 1
# so we need to do the first one
# in order to have values in rank and previous rank
begin
  puts "Iteration #{iteration}"
  # give the rank_pages method a load factor
  ranker.rank_pages(0.85)
  # show me results !
  ranker.show_ranks
  iteration += 1
  puts
# well we end our loop when results are converging
end until ranker.converging?
